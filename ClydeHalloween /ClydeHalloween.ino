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


// -- clyde dev
ClydeDev clyde = ClydeDev();


// -- touchy feely module in module port 2
ClydeTouchyFeely touchyfeely = ClydeTouchyFeely(2);
boolean tf_enabled = false;


// -- pins
uint8_t spkr = 4;


// -- behaviour
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


// -- halloween!

// leg touches
uint8_t pause_thresh = 250;
long last_touch_time[] = { 0, 0, 0, 0 };
boolean legTouched[] = { false, false, false, false };
boolean allReleased = false;

// misc
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
  
  // start the pins
  pinMode(spkr, OUTPUT);
  
  
  // this is how clyde beeps hello! beep beep
  playTone(300, 100);
  playTone(400, 100);
  playTone(500, 100);
  delay(100);
  playTone(300, 100);
  delay(50);
  playTone(300, 100);
  
  
  Serial.begin(9600);
  while(!Serial);
  
  delay(5000);
  
  // -- clyde!
  clyde.init();
  
  clyde.setEyePressedHandler(clydeEyePressed);
  clyde.setEyeReleasedHandler(clydeEyeReleased);
  
  
  // -- touchy feely module
  touchyfeely.setDebugStream(&Serial, ClydeDev::DEBUG); // uncomment if you want to see debug text
  tf_enabled = touchyfeely.init();
  
  if(tf_enabled) {
    touchyfeely.setTouchedHandler(clydeTouched);
    touchyfeely.setReleasedHandler(clydeReleased);
    touchyfeely.setDetectedHandler(clydeDetected);
  }
  
  
  // -- initialised
  Serial << "Hello! :3" << endl;
  
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  clyde.setLight(light_num);
  
  
}


void loop() {
  
  clyde.update();
  
  if(tf_enabled) touchyfeely.update();
  
  if(millis()-last_bloop >= bloop_every && blink_block == false) {
    
    for(uint8_t i=0; i<3; i++) {
      playTone( (int)random(100, 1500), 100 );
      delay(50);
    }
    
    last_bloop = millis();
    
  }
  
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


