/*
 * Clyde Halloween
 * ------------
 *
 * Trick or Treat? Clyde will answer!
 *
 * Piezo speaker: Pin D4
 * Touchy Feely Module: Spot #2
 *
 * Note: in the ClydeDev library - ClydeTouchyFeely.h, we changed this:
 * TOUCH_LEVEL = 0x09;
 * just for this example.
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
 * October 16, 2014
 *
 */

#include <Wire.h>
#include <MPR121.h>
#include <Streaming.h>

#include <ClydeDev.h>
#include <ClydeTouchyFeely.h>


// set up all of our variables that we will be using. you will see when they are used
// in the code below, and in the other tabs. so check it out! :)

// clyde dev
ClydeDev clyde = ClydeDev();


// touchy feely module in module port 2
ClydeTouchyFeely touchyfeely = ClydeTouchyFeely(2);
boolean tf_enabled = false;


// pins of our outputs and inputs
uint8_t spkr = 4;


// behaviour related variables
float hue_num = ClydeDev::hue_red;
int light_num = 10;
boolean hue_dir = true;
boolean light_dir = true;
boolean red_dir = true;
boolean green_dir = true;
boolean blue_dir = true;
float intensity = 0.3;
boolean currently_pressed = false;
uint8_t start_r = 0;
uint8_t start_g = 0;
uint8_t start_b = 0;


// leg touches
uint8_t pause_thresh = 250;
long last_touch_time[] = { 0, 0, 0, 0 };
boolean legTouched[] = { false, false, false, false };
boolean allReleased = false;


// misc, halloween related
long last_bloop = 0;
uint16_t bloop_every = 3000;
long last_blink = 0;
uint8_t blink_every = 100;
uint8_t blinky_num = 0;
float chosen_hue = 0.0;
boolean blink_block = false;
float hue_redzorz = 0.0;
float hue_orangezorz = 8.0;


void setup() {
  
   // set up the output and input pins
  pinMode(spkr, OUTPUT);
  
  
  // this is how clyde beeps hello! beep beep
  // play this at the start just for fun
  playTone(300, 100);
  playTone(400, 100);
  playTone(500, 100);
  delay(100);
  playTone(300, 100);
  delay(50);
  playTone(300, 100);
  
  
  Serial.begin(9600);
  while(!Serial); // we need this part to make sure that serial does not become 'borked'
  delay(5000); // this delay is to also help with the general 'de-borking' of serial
  
  
  clyde.init();
  
  
  // setting out eye callback methods
  clyde.setEyePressedHandler(clydeEyePressed);
  clyde.setEyeReleasedHandler(clydeEyeReleased);
  
  
  tf_enabled = touchyfeely.init();
  
  // setting the leg touch callback methods
  if(tf_enabled) {
    touchyfeely.setTouchedHandler(clydeTouched);
    touchyfeely.setReleasedHandler(clydeReleased);
    touchyfeely.setDetectedHandler(clydeDetected);
  }
  
  
  Serial << "Hello! :3" << endl;
  
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  clyde.setLight(light_num);
  
}


void loop() {
  
  // update clyde each loop iteration, to make sure its sensors stay fresh
  clyde.update();
  
  // if we are enabling touchy feely, then make sure to update it too
  if(tf_enabled) touchyfeely.update();
  
  // make clyde do a 'bloop' every few seconds, as long as blink_block is false.
  // we set blink_block to true whenever we update the leds to a new colour,
  // and so by checking that it will be false, then we can prevent this behaviour
  // from interrupting the expected behaviour. in this case, it is just for
  // preventing the sounds from playing.
  if(millis()-last_bloop >= bloop_every && blink_block == false) {
    
    for(uint8_t i=0; i<3; i++) {
      playTone( (int)random(100, 1500), 100 );
      delay(50);
    }
    
    last_bloop = millis();
    
  }
  
  // same as above, but this time instead of sounds, it is for changing the eye
  // colour.
  if(millis()-last_blink >= blink_every && blink_block == false) {
    
    // change the hue every 4 blinks
    if(blinky_num % 4 == 0) {
      if(headsOrTails()) {
        chosen_hue = hue_redzorz;
      } else {
        chosen_hue = hue_orangezorz;
      }
      blinky_num = 0;
    }
    
    // every 2nd blink, make it go dim
    if(blinky_num % 2 == 0) {
      clyde.setEyeHSI(chosen_hue, 1.0, 0.5);
    } else {
      clyde.setEyeHSI(chosen_hue, 1.0, 1.0);
    }
    
    blinky_num++;
    last_blink = millis();
    
  }
  
}


