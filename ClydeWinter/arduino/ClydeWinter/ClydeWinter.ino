/*
 * Clyde Winter
 * ------------
 *
 * Clyde loves to celebrate winter, even though it is a robot!
 * It's all decked out with 3d printed snowflakes and is ready
 * to display the internet-connected cheerlights feed!
 *
 * Speaker: Pin D4
 * ST_CP of 74HC595: Pin D10
 * SH_CP of 74HC595: Pin D12
 * DS of 74HC595: Pin D13
 *
 * For more info on the 74HC595 shift out chip, check out
 * this tutorial:
 * --> http://www.arduino.cc/en/Tutorial/ShiftOut
 *
 * This project uses the ClydeDev library. For info on
 * how to install this, please see:
 * --> https://github.com/RobotGrrl/ClydeDev
 * --> https://github.com/fabule
 *
 * For more info about Clyde:
 * --> http://fabule.com
 *
 * By Erin RobotGrrl
 * --> http://robotgrrl.com
 *
 * December 20, 2014
 *
 */

#include <Wire.h>
#include <MPR121.h>
#include <Streaming.h>
#include <NewPing.h>

#include <ClydeDev.h>
#include <ClydeTouchyFeely.h>
#include <ClydeAfraidDark.h>


// set up all of our variables that we will be using. you will see when they are used
// in the code below, and in the other tabs. so check it out! :)

// some of our behaviour defaults
#define BRIGHTNESS 30
#define PLAY_SOUND false


// clyde dev
ClydeDev clyde = ClydeDev();


// touchy feely module in module port 2
ClydeTouchyFeely touchyfeely = ClydeTouchyFeely(2);
boolean tf_enabled = false;


// pins of our outputs and inputs
uint8_t spkr = 4;

//Pin connected to ST_CP of 74HC595
int latchPin = 10;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 13;


// misc
long current_time = 0;
long last_heartbeat = 0;
int colour_index = 99;
long last_sing = 0;
long sing_every = 3000UL;//1000UL * 60;
boolean carolling = false;


// behaviour related variables
float hue_num = ClydeDev::hue_green;
int light_num = BRIGHTNESS;
float intensity = 0.6;


// colours
int all_colour_rgb[][3] = {
  {255, 0, 0},
  {0, 255, 0},
  {0, 0, 255},
  {0, 255, 255},
  {255, 255, 255},
  {250, 249, 202},
  {255, 0, 180},
  {255, 50, 100},
  {255, 255, 0},
  {255, 150, 0},
  {255, 50, 50},
  {250, 249, 202}
};


// snowflake lights
byte data;
byte dataArray[10];
boolean bloop = true;
long last_snowflake_update = 0;


void setup() {
  
  Serial.begin(9600);
  while(!Serial); // we need this part to make sure that serial does not become 'borked'
  delay(5000); // this delay is to also help with the general 'de-borking' of serial
  
  
  // set up our io pins
  pinMode(spkr, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  
  // lets begin clyde
  clyde.init();
  
  
  // setting out eye callback methods
  clyde.setEyePressedHandler(clydeEyePressed);
  clyde.setEyeReleasedHandler(clydeEyeReleased);
  
  
  // snowflake lights
  dataArray[0] = B11111111;
  dataArray[1] = B10101010;
  dataArray[2] = B01010101;
  
  
  // let's set the snowflake lights to be all on at the beginning
  data = dataArray[0];
  //ground latchPin and hold low for as long as you are transmitting
  digitalWrite(latchPin, 0);
  //move 'em out
  shiftOut(dataPin, clockPin, LSBFIRST, data);
  //return the latch pin high to signal chip that it 
  //no longer needs to listen for information
  digitalWrite(latchPin, 1);
  
  clyde.setEyeHSI(hue_num, 0.0, intensity);
  clyde.setLight(BRIGHTNESS);
  
  //Serial << "Hello! :3" << endl;
  
}

void loop() {
  
  // set the current time as a variable so that way we don't have to call millis() all the time
  // during the same loop iteration...
  current_time = millis();

  
  // update clyde each loop iteration, to make sure its sensors stay fresh
  clyde.update();
  
  
  // check for any data from the processing sketch
  if(Serial.available() > 0) {
    
    // receive the data as an int
    int a = Serial.parseInt();
    
    // as long as it is not a 0, then it is a new colour
    if(a != 0) {
      colour_index = a-1;
      //Serial << colour_index << endl;
    }
    
  }
  
  
  // make sure we send a heartbeat for the processing sketch to listen for!
  if(current_time-last_heartbeat >= 1000 * 2) {
    Serial.print("~");
    last_heartbeat = current_time;
  }
  
  
  // set the colours
  clyde.setEyeRGB( all_colour_rgb[colour_index][0], all_colour_rgb[colour_index][1], all_colour_rgb[colour_index][2] );
  clyde.setLight(BRIGHTNESS);
  
  
  // update the snowflake leds
  if(current_time-last_snowflake_update >= 250) {
    
    // choosing our light sequence
    if(bloop) {
      data = dataArray[1];
    } else {
      data = dataArray[2];
    }
    
    // setting the leds to the shift register
    
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, 0);
    //move 'em out
    shiftOut(dataPin, clockPin, LSBFIRST, data);
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    
    bloop = !bloop;
    last_snowflake_update = current_time;
    
  }
  
  
  // let clyde sing every bit of time (the time interval is set in sing_every)
  if(current_time-last_sing >= sing_every && carolling == true) {
    
    int startFreq = (int)random(500, 1200);
    
    for(int i=0; i<2; i++) {
      playTone(startFreq, 100);
      delay(50);
  
      playTone(startFreq+250, 50);
      delay(50);    
      
      playTone(startFreq-250, 50);
      delay(50);
    }
    
    playTone(startFreq, 100);
    delay(50);
    
    for(uint16_t i=startFreq; i<(startFreq+250); i+=50) {
      playTone(i, 20);
    }
    
    last_sing = current_time;
    
  }
  
  
}



// code snippet from: http://arduino.cc/en/Tutorial/ShftOut12
// This method sends bits to the shift register:
void registerWrite(int whichPin, int whichState) {
// the bits you want to send
  byte bitsToSend = 0;

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);

  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);

  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

}


