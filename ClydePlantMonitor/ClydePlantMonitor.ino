/*
 * Clyde Flower Monitor
 * --------------------
 *
 * Clyde simply love plants, and wants to help take care of your
 * flowers for you!
 *
 * Soil moisture sensor: Pin A3
 * Piezo speaker: Pin D4
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
 * November 3, 2014
 *
 */

#include <Wire.h>
#include <MPR121.h>
#include <Streaming.h>

#include <ClydeDev.h>
#include <ClydeTouchyFeely.h>
#include <ClydeAfraidDark.h>


// set up all of our variables that we will be using. you will see when they are used
// in the code below, and in the other tabs. so check it out! :)


// some of our behaviour defaults
#define BRIGHTNESS 0
#define PLAY_SOUND false
#define SEND_SERIAL true


// clyde dev
ClydeDev clyde = ClydeDev();


// pins of our outputs and inputs
uint8_t moisture_sensor = A3;
uint8_t spkr = 4;


// soil moisture sensor threshold levels
#define DRY_SOIL 300
#define HUMID_SOIL 700
#define MOIST_SOIL 950


// plant related variables
boolean USE_SENSOR = true;

uint16_t moisture_level = 0;
uint8_t previous_thresh = 99;
uint8_t current_thresh = 99;
long last_sound = 0;
long last_serial = 0;

float hue_bound_a = 0.0; // smaller
float hue_bound_b = 360.0; // bigger
uint8_t hue_increment = 1;


// behaviour related variables
float hue_num = ClydeDev::hue_green;
int light_num = 10;
boolean hue_dir = true;
boolean light_dir = true;
boolean red_dir = true;
boolean green_dir = true;
boolean blue_dir = true;
float intensity = 0.3;


void setup() {
  
  Serial.begin(9600);
  while(!Serial); // we need this part to make sure that serial does not become 'borked'
  delay(5000); // this delay is to also help with the general 'de-borking' of serial
  
  
  // set up our io pins
  pinMode(moisture_sensor, INPUT);
  pinMode(spkr, OUTPUT);
  
  
  clyde.init();
  
  
  // setting out eye callback methods
  clyde.setEyePressedHandler(clydeEyePressed);
  clyde.setEyeReleasedHandler(clydeEyeReleased);
  
  
  Serial << "Hello! :3" << endl;
  
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  clyde.setLight(BRIGHTNESS);
  
}

void loop() {
  
  // update clyde each loop iteration, to make sure its sensors stay fresh
  clyde.update();
  
  
  // check our serial monitor, if there is anything in the buffer, use
  // serialFunction() to look at it
  if(Serial.available() > 0) serialFunction();
  
  
  // this was mainly for debugging purposes. in reality, USE_SENSOR will
  // always be true.
  if(USE_SENSOR) {
  
    // reading the sensor!
    moisture_level = analogRead(moisture_sensor);
    
    
    // here we are classifying the sensor reading into four different threshold levels
    // (current_thresh), so that we can do some fun things with this later on
    if(moisture_level <= DRY_SOIL) {
      current_thresh = 0;
    } else if(moisture_level > DRY_SOIL && moisture_level <= HUMID_SOIL) {
      current_thresh = 1;
    } else if(moisture_level > HUMID_SOIL && moisture_level <= MOIST_SOIL) {
      current_thresh = 2;
    } else {
      current_thresh = 3;
    }
  
  }
  
  
  // check to see if the moisture level has downgraded to a different threshold,
  // and also make sure that it isn't the initial values (this is what the 99 is for)
  if(previous_thresh != current_thresh && previous_thresh != 99 && current_thresh != 99) {
    
    // let us do a little animation to signify the change to the user!
    
    for(int i=0; i<5; i++) {
      
      if(i%2 == 0) {
        clyde.setLight(30);
      } else {
        clyde.setLight(200);
      }
      
      delay(100);
      
    }
    
    clyde.setLight(BRIGHTNESS);
    
  }
  
  
  // now we can do the ambient behaviour based on what threshold the reading is in.
  // for example, change the eye led's colour, or play some sound, etc.
  switch(current_thresh) {
    
    case 0:
    
      // this same sort of template is repeated for each one of the cases. so the
      // comments in this one can apply to the other ones too, in general!
    
      // check to see when we last sent a serial message. if it was more than 1s ago,
      // send the string to serial again and set the new time.
      if(millis()-last_serial >= 1000 && SEND_SERIAL == true) {
        Serial << "You should water the plant! (" << moisture_level << ")" << endl;
        last_serial = millis();
      }
    
      
      // set our colour preferences for this behaviour
      hue_bound_a = 0.0;
      hue_bound_b = 20.0;
      hue_increment = 1;
      
      
      // check to see when we last played a sound, and if our default settings are
      // indeed to play a sound. then play the sound and set the new time.
      if(millis()-last_sound >= 3000 && PLAY_SOUND == true) {
        playTone( 500, 100 );
        delay(50);
        playTone( 700, 100 );
        delay(50);
        playTone( 900, 100 );
        last_sound = millis();
      }
      
    break;
    
    case 1:
    
      if(millis()-last_serial >= 1000 && SEND_SERIAL == true) {
        Serial << "Your plant might need to be watered soon! (" << moisture_level << ")" << endl;
        last_serial = millis();
      }
      
      hue_bound_a = 30.0;
      hue_bound_b = 80.0;
      hue_increment = 5;
      
      if(millis()-last_sound >= 3000 && PLAY_SOUND == true) {
        playTone( 1000, 100 );
        playTone( 700, 100 );
        playTone( 400, 100 );
        last_sound = millis();
      }
      
    break;
    
    case 2:
    
      if(millis()-last_serial >= 1000 && SEND_SERIAL == true) {
        Serial << "Your plant has been watered recently- well done! (" << moisture_level << ")" << endl;
        last_serial = millis();
      }
    
      hue_bound_a = 80.0;
      hue_bound_b = 110.0;
      hue_increment = 1;
      
      if(millis()-last_sound >= 5000 && PLAY_SOUND == true) {
        playTone( 1000, 50 );
        delay(50);
        playTone( 1000, 50 );
        last_sound = millis();
      }
    
    break;
    
    case 3:
    
      if(millis()-last_serial >= 3000 && SEND_SERIAL == true) {
        Serial << "Something is wrong-- too much water. Did you drown your plant or something? (" << moisture_level << ")" << endl;
        last_serial = millis();
      }
    
      hue_bound_a = 180.0;
      hue_bound_b = 240.0;
      hue_increment = 2;
      
      if(millis()-last_sound >= 3000 && PLAY_SOUND == true) {
        playTone( 300, 200 );
        delay(50);
        playTone( 300, 100 );
        delay(50);
        playTone( 300, 50 );
        last_sound = millis();
      }
    
    break;
    
  }
  
    
  // we have to set the previous to the current to prepare for the next loop iteration
  previous_thresh = current_thresh;
  
  
  // here we are making the eye light cycle between the two given hue boundaries.
  // the hue boundaries are set according to the threshold level (see block of code
  // above). 
  if(hue_dir) {
    
    if(hue_num >= hue_bound_b) {
      hue_dir = !hue_dir;
      hue_num -= hue_increment;
    } else {
      hue_num += hue_increment;
    }
    
  } else {
    
    if(hue_num <= hue_bound_a) {
      hue_dir = !hue_dir;
      hue_num += hue_increment;
    } else {
      hue_num -= hue_increment;
    }
    
  }
  
  clyde.setEyeHSI(hue_num, 1.0, 1.0);
  
  
  // we have to take a bit of a rest, before reading the analog sensor again
  // (ps: it won't be changing very quickly anyway! :P)
  delay(20);
  
}


// this is just for testing / filming, so we can easily change the current thresh if
// we need to.
void serialFunction() {
  
  Serial.print("hi");
  
  char c = Serial.read();

  if(c == '0') {
    current_thresh = 0;
  } else if(c == '1') {
    current_thresh = 1;
  } else if(c == '2') {
    current_thresh = 2;
  } else if(c == '3') {
    current_thresh = 3;
  } else if(c == 'e') {
    USE_SENSOR = !USE_SENSOR;
  }
 
}


