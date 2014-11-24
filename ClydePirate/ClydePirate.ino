/*
 * Clyde Pirate
 * ------------
 *
 * Clyde can help you protect treasure! Yarr!
 *
 * Ultrasonic sensor: Pin D5
 * Piezo speaker: Pin D4
 * Laser module: Pin D6
 * Touchy Feely Module: Spot #2
 *
 * For the ultrasonic sensor, we use the NewPing library.
 * Check it out here:
 * --> http://playground.arduino.cc/Code/NewPing
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
uint8_t ping_sensor = 10;
uint8_t spkr = 4;
uint8_t laser = 12;


// misc
long current_time = 0;


// ultrasonic sensor related
#define TRIGGER_PIN  10  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

long last_ping_read = 0;     // last time we read the sensor
uint8_t dist_thresh = 10;    // (in cm)
uint16_t dist_time = 3000;   // (in ms)
uint8_t alarm_mode = 0;      // 0 for alarm, 1 for reward
boolean lock_on = false;     // if we are locked on to a target (it's past the threshold)
long lock_start = 0;         // start time of the lock


// behaviour related variables
float hue_num = ClydeDev::hue_green;
int light_num = BRIGHTNESS;
float intensity = 0.6;


void setup() {
  
  Serial.begin(9600);
  while(!Serial); // we need this part to make sure that serial does not become 'borked'
  delay(5000); // this delay is to also help with the general 'de-borking' of serial
  
  
  // set up our io pins
  pinMode(ping_sensor, INPUT);
  pinMode(spkr, OUTPUT);
  pinMode(laser, OUTPUT);
  
  
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
  clyde.setLight(BRIGHTNESS);
  digitalWrite(laser, LOW);
  
}

void loop() {
  
  // set the current time as a variable so that way we don't have to call millis() all the time
  // during the same loop iteration...
  current_time = millis();
  
  
  // update clyde each loop iteration, to make sure its sensors stay fresh
  clyde.update();
  
  
  // if we are enabling touchy feely, then make sure to update it too
  if(tf_enabled) touchyfeely.update();
  
  
  // checking to see if we should read the sensor again
  if(current_time-last_ping_read >= 100) {
    
    unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
    int ping_val = (int)(uS / US_ROUNDTRIP_CM);
    Serial << "Ping: " << ping_val << endl;
    
    // let's see if we are already locked on to a 'target' or not (for lack of better term)
    if(!lock_on) {
      
      // check to see if the target is at the thresh (or closer)
      // if it is, then set the lock_start time to now
      if(ping_val <= dist_thresh) {
        lock_on = true;
        lock_start = current_time;
      }
      
    } else {
      
      // if the target ventures beyond the thresh, then turn the
      // lock to false
      if(ping_val > dist_thresh) {
        lock_on = false;
      }
      
      // if the target has been within the thresh for the set amount of time, then we will
      // perform the given action for the alarm or reward
      if(current_time-lock_start >= dist_time) {
        
        if(alarm_mode == 0) { // alarm
        
          // all of the function calls below are for creating an interesting animation
          // for this alarm mode! fun!
        
          clyde.setEyeHSI(0.0, 1.0, 1.0);
          clyde.setLight(200);
          
          for(uint8_t i=0; i<5; i++) {
            
            if(i%2 == 0) {
              digitalWrite(laser, LOW);
            } else {
              digitalWrite(laser, HIGH);
            }
            
            playTone(400, 100);
            delay(100);
          }
          
          digitalWrite(laser, LOW);
          
          for(uint8_t i=0; i<10; i++) {
            playTone(200, 10);
            delay(50);
          }
          
          for(uint8_t i=200; i>60; i--) {
            clyde.setLight(i);
            delay(4);
          }
          
          for(uint8_t i=0; i<10; i++) {
            playTone(300, 10);
            delay(50);
          }
          
          for(uint8_t i=60; i<200; i++) {
            clyde.setLight(i);
            delay(4);
          }
          
          for(uint8_t i=0; i<10; i++) {
            playTone(400, 10);
            delay(50);
          }
          
          for(uint8_t i=0; i<5; i++) {
            
            if(i%2 == 0) {
              digitalWrite(laser, LOW);
            } else {
              digitalWrite(laser, HIGH);
            }
            
            playTone(400, 100);
            delay(100);
          }
          
          digitalWrite(laser, LOW);
          clyde.setLight(BRIGHTNESS);
          
        } else if(alarm_mode == 1) { // reward
          
          // all of the function calls below are for creating an interesting animation
          // for this reward mode! fun!
          
          digitalWrite(laser, HIGH);
          
          playTone(800, 50);
          delay(100);
          playTone(800+200, 50);
          delay(100);
          playTone(800+400, 50);
          
          for(uint8_t i=0; i<12; i++) {
            // sparkle the light
            if(i%2 == 0) {
              clyde.setLight(30);
            } else {
              clyde.setLight(200);
            }
            delay(100);
          }
          
          clyde.setLight(BRIGHTNESS);
          
          for(int i=800; i<800+500; i+=50) {
            playTone(i, 20);
          }
          
          for(uint16_t i=0; i<90; i++) {
            clyde.setEyeHSI((float)(i*4), 1.0, intensity);
            delay(20);
          }
          
          for(int i=800+500; i>800; i-=50) {
            playTone(i, 20);
          }
          
          for(uint16_t i=0; i<90; i++) {
            clyde.setEyeHSI((float)(i*4), 1.0, intensity);
            delay(20);
          }
          
          playTone(1000, 100);
          delay(100);
          playTone(1200, 100);
          delay(100);
          playTone(1400, 100);
          delay(100);
          
          digitalWrite(laser, LOW);
          
        }
        
        lock_on = false;
        
      }
      
    }
    
    last_ping_read = millis();
    
  }
  
  
  // makes the eye led look like it's going BLUB BLUB (like a heartbeat)
  uint16_t slice = millis() % 1000;
  if(slice < 100 || (slice > 200 && slice < 300)) {
    clyde.setEyeHSI(260.0, 1.0, intensity);
  } else {
    clyde.setEyeHSI(180.0, 1.0, intensity);
  }
  
  
}



