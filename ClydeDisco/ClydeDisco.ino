/*
 * Clyde Disco
 * ------------
 *
 * Let's get Clyde dancing to the music! WOOT!
 *
 * Piezo speaker: Pin D4
 * Microphone: Pin A3
 * Touchy Feely Module: Spot #2
 * Afraid of the Dark Module: Spot #1
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


// set up all of our variables that we will be using. you will see when they are used
// in the code below, and in the other tabs. so check it out! :)

// clyde dev
ClydeDev clyde = ClydeDev();


// touchy feely module in module port 2
ClydeTouchyFeely touchyfeely = ClydeTouchyFeely(2);
boolean tf_enabled = false;


// afraid of the dark module in module port 1
ClydeAfraidDark afraiddark = ClydeAfraidDark(1);
boolean ad_enabled = false;


// pins of our outputs and inputs
uint8_t spkr = 4;
uint8_t mic = A3;


// behaviour related variables
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


// songs related variables
uint8_t current_song = 0;
uint8_t num_songs = 3;


// theremin related variables
uint16_t upper_light = 900;
uint16_t lower_light = 100;
boolean light_sound_synth = false;
long last_light_change = 0;


// leg touches variables
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

 
// mic related variables
long last_silence = 0;
long last_added_humm = 0;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;



void setup() {
  
  // set up the output and input pins
  pinMode(spkr, OUTPUT);
  pinMode(mic, INPUT);
  
  
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
  
  
  ad_enabled = afraiddark.init();
  
  
  Serial << "Hello! :3" << endl;
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  clyde.setLight(light_num);
  
}


void loop() {
  
  // sample the microphone continuously
  micSample();
  
  // update clyde each loop iteration, to make sure its sensors stay fresh
  clyde.update();
  
  // if we are enabling touchy feely, then make sure to update it too
  if(tf_enabled) touchyfeely.update();
  
  // if we are enabling the afraid of the dark module, and the light sound
  // synth feature is on, then we will update the module
  if(ad_enabled && light_sound_synth) {
    
    afraiddark.update();
    
    uint16_t light_level = afraiddark.getLight();
    
    // check to make sure the light level is within the boundaries
    if(light_level < lower_light) light_level = lower_light;
    if(light_level > upper_light) light_level = upper_light;
    
    // using the boundaries, we can map the light level to a pitch value,
    // and send it to the speaker
    int thisPitch = map(light_level, 100, 1200, lower_light, upper_light);
    playTone(thisPitch, 50);
    
    cycleHue();
    
  }
  
  // task_cycle is set to true whenever the 2nd pattern sequence is matched.
  // it cycles the light, or you can remove that line and uncomment out the
  // block to make it shine random brightness levels of light
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


