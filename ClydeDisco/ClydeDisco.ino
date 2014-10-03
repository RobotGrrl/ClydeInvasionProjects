/*
 * Clyde Disco
 * ------------
 *
 * Let's get Clyde dancing to the music! WOOT!
 *
 * Piezo speaker: Pin D4
 * Microphone: Pin A3
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
 * September 29, 2014
 *
 */

#include <Wire.h>
#include <MPR121.h>
#include <Streaming.h>

#include <ClydeDev.h>
#include <ClydeTouchyFeely.h>
#include <ClydeAfraidDark.h>


// -- clyde dev
ClydeDev clyde = ClydeDev();


// -- touchy feely module in module port 2
ClydeTouchyFeely touchyfeely = ClydeTouchyFeely(2);
boolean tf_enabled = false;


// afraid of the dark module in module port 1
ClydeAfraidDark afraiddark = ClydeAfraidDark(1);
boolean ad_enabled = false;

// -- pins
uint8_t spkr = 4;
uint8_t mic = A3;


// -- behaviour
long last_blink = 0;
boolean blink_on = false;
float hue_num = ClydeDev::hue_green;
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


// -- disco!

// songs
uint8_t current_song = 0;
uint8_t num_songs = 3;

// theremin
uint16_t upper_light = 900;
uint16_t lower_light = 100;
boolean light_sound_synth = false;
long last_light_change = 0;

// leg touches
uint8_t touch_tone_duration = 100;
uint8_t pause_thresh = 250;
boolean swoopy_sounds = false;
boolean task_cycle = false;
long last_touch_time[] = { 0, 0, 0, 0 };
boolean legTouched[] = { false, false, false, false };
boolean allReleased = false;
uint8_t legPresses[] = { 9, 9, 9, 9 };
const uint8_t numPatterns = 2;
uint8_t legPattern[numPatterns][4] = { { 0, 1, 2, 3 },
                                       { 3, 3, 3, 3 } };
 
// mic
long last_silence = 0;
long last_added_humm = 0;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;



void setup() {
  
  // start the pins
  pinMode(spkr, OUTPUT);
  pinMode(mic, INPUT);
  
  
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
  tf_enabled = touchyfeely.init();
  
  if(tf_enabled) {
    touchyfeely.setTouchedHandler(clydeTouched);
    touchyfeely.setReleasedHandler(clydeReleased);
    touchyfeely.setDetectedHandler(clydeDetected);
  }
  
  
  // -- afraid of the dark module
  ad_enabled = afraiddark.init();
  
  
  // -- initialised
  Serial << "Hello! :3" << endl;
  
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  clyde.setLight(light_num);
  
}


void loop() {
  
  micSample();
  
  clyde.update();
  
  if(tf_enabled) touchyfeely.update();
  
  if(ad_enabled && light_sound_synth) {
    
    afraiddark.update();
    
    uint16_t light_level = afraiddark.getLight();
    
    if(light_level < lower_light) light_level = lower_light;
    if(light_level > upper_light) light_level = upper_light;
    
    int thisPitch = map(light_level, 100, 1200, lower_light, upper_light);
    playTone(thisPitch, 50);
    
    cycleHue();
    
  }
  
  if(task_cycle) {
    /*
    if(millis()-last_light_change >= 300) {
      clyde.setLight((int)random(0, 256));
      last_light_change = millis();
    }
    */
    cycleLight();
  }
  
}


