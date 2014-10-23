// this is the callback function that is called whenever one of clyde's legs is touched
void clydeTouched(uint8_t l) {
 
  Serial << "Callback- Touched! " << l << endl;
  
  // gotta check which leg was touched. for 0, 1, 2, and 5, the format is the same
  switch(l) {
    case 0: {
      
      // this is to help make the numbering of the legs less confusing IRL. so even though
      // it is wired to pin 0, this is actually leg #1. it helps with debugging, trust me
      uint8_t leg_num = 1;
    
      // if they are holding the leg, pause for a bit before running this behaviour
      if(millis() - last_touch_time[leg_num] >= pause_thresh) {
        
        hue_num = 0.0;
        clyde.setEyeHSI(hue_num, 1.0, intensity);
        
        playTone(800, touch_tone_duration);
        
        if(swoopy_sounds) { // swoopy_sounds becomes true when they unlock one of the patterns
          for(int i=800; i<800+1000; i+=100) {
            playTone(i, 10);
          } 
        }
        
        // push this leg number onto the stack (which is used later for detecting the patterns)
        newLegPressed(leg_num); 
        last_touch_time[leg_num] = millis();
      
      }
      
      legTouched[leg_num] = true; // this is used later to check if all of the legs are released
    
    }
    break;
    case 1: { // please see case 0 for the comments- it is the same template!
    
      uint8_t leg_num = 2;
    
      if(millis() - last_touch_time[leg_num] >= pause_thresh) {
        
        hue_num = 165.0;
        clyde.setEyeHSI(hue_num, 1.0, intensity);
        
        playTone(600, touch_tone_duration);
        
        if(swoopy_sounds) {
          for(int i=600; i<600+1000; i+=100) {
            playTone(i, 10);
          } 
        }
        
        newLegPressed(leg_num);
        last_touch_time[leg_num] = millis();
        
      }
      
      legTouched[leg_num] = true;
      
    }
    break;
    case 2: { // please see case 0 for the comments- it is the same template!
      
      uint8_t leg_num = 3;
    
      if(millis() - last_touch_time[leg_num] >= pause_thresh) {
        
        hue_num = 75.0;
        clyde.setEyeHSI(hue_num, 1.0, intensity);
        
        playTone(400, touch_tone_duration);
        
        if(swoopy_sounds) {
          for(int i=400; i<400+1000; i+=100) {
            playTone(i, 10);
          } 
        }
        
        newLegPressed(leg_num);
        last_touch_time[leg_num] = millis();
        
      }
      
      legTouched[leg_num] = true;
      
    }
    break;
    case 3: {
      // this leg is used to turn on and off the light-to-sound synth
     
     for(int i=500; i<1000; i+=100) {
       playTone(i, 10);
     }
      
    }  
    break;
    case 4:
      // no connection to this leg
    break;
    case 5: { // please see case 0 for the comments- it is the same template!
    
      uint8_t leg_num = 0;
    
      if(millis() - last_touch_time[leg_num] >= pause_thresh) {
        
        hue_num = 270.0;
        clyde.setEyeHSI(hue_num, 1.0, intensity);
        
        playTone(1000, touch_tone_duration);
        
        if(swoopy_sounds) {
          for(int i=1000; i<1000+1000; i+=100) {
            playTone(i, 10);
          } 
        }
        
        newLegPressed(leg_num);
        last_touch_time[leg_num] = millis();
        
      }
      
      legTouched[leg_num] = true;
      
    }
    break;
  }
  
}


// this is a callback function for the touchy feely module. it is called whenever one of clyde's
// legs is released
void clydeReleased(uint8_t l) {
 
  //Serial << "Callback- Released!" << l << endl;
  
  uint8_t leg_num = 8; // initialise with a dummy val
  
  switch(l) {
    case 0: {
      leg_num = 1;
      legTouched[leg_num] = false;
    }
    break;
    case 1: {
      leg_num = 2;
      legTouched[leg_num] = false;
    }
    break;
    case 2: {
      leg_num = 3;
      legTouched[leg_num] = false;
    }
    break;
    case 3: {
      leg_num = 9;
      
      light_sound_synth = !light_sound_synth;
     
      for(int i=1000; i>500; i-=100) {
      //  playTone(i, 10);
      }
      
    }
    break;
    case 4:
      // no connection to this leg
    break;
    case 5: {
      leg_num = 0;
      legTouched[leg_num] = false;
    }
    break;
  }
  
  // here we are checking if all of the legs have been released. whenever a leg is
  // touched, it sets legTouched[i] to true, and when released to false, so it is
  // easy to check this.
  allReleased = true;
  for(int i=0; i<4; i++) {
    if(legTouched[i]) {
      allReleased = false;
      break;
    }
  }
  
  // we gotta catch this edge case! leg_num 9 is the leg for turning on/off the theremin
  // behaviour. if this one has been released, we do not actually want it to do any of
  // the same stuff as the other legs (like blink the eye and check the patterns -- see
  // below), so we can get around this by sneakily setting allReleased to false here.
  if(leg_num == 9) allReleased = false;
  
  
  if(allReleased) { // if all of the legs have been released, now we check for the pattern matching
    
    for(int i=0; i<3; i++) {
      clyde.setEyeHSI(hue_num, 0.0, intensity);
      delay(100);
      clyde.setEyeHSI(hue_num, 1.0, intensity);
      delay(100);
    }
    
    clyde.setEyeHSI(hue_num, 0.0, intensity);
   
    checkLegPatterns();
    
  }
  
}


// stack of leg presses
void newLegPressed(uint8_t l) {
  
  legPresses[3] = legPresses[2];
  legPresses[2] = legPresses[1];
  legPresses[1] = legPresses[0];
  legPresses[0] = l;
  
}


// pattern matching algorithm for the leg press patterns. this is called after all of the legs
// have been released, and it checks the stack of leg presses to see if the most recent ones
// match (in order), a defined pattern. the patterns are defined as global vars, so see the main
// sketch file at the top to see them.
void checkLegPatterns() {
 
 for(int j=0; j<numPatterns; j++) {
   
   boolean match = true;
   
   for(int i=0; i<4; i++) {
   
     Serial << legPresses[3-i] << " vs " << legPattern[j][i] << endl;
     
     // if the *oldest* leg press does not match the *first* leg touch in the pattern, it is not
     // a match. do you get it? we have to go backwards, otherwise the human would have to
     // enter in the pattern backwards. so since clyde is a robot, we can help make this easier
     // for the humans out there.
     if(legPresses[3-i] != legPattern[j][i]) {
       match = false;
       break;
     }
   
   }
   
   if(match) patternMatched(j); // yea! there was a match!
   
 }
  
}


// called whenever a pattern has been detected!
void patternMatched(uint8_t p) {
  Serial << "the #" << p << " pattern matched" << endl;
  
  // first, let us celebrate that the human has matched a pattern. celebration!
  for(int i=0; i<=12; i++) {
    
    if(headsOrTails()) {
      clyde.setEyeHSI(hue_num, 0.0, intensity);
    } else {
      clyde.setEyeHSI(hue_num, 1.0, intensity);
    }
    
    if(headsOrTails()) {
      clyde.setLight(30);
    } else {
      clyde.setLight(200);
    }
    
    if(i%3 == 0) {
      
      uint16_t hoot = (int)random(200, 1000);
    
      for(int i=hoot; i<hoot+500; i+=50) {
         playTone(i, 10);
       }
      
    } else {
      delay(100); 
    }
    
  }
  
  
  // unlock new features based on what pattern was matched. and lock them again
  // if the pattern is matched again. why? i don't know. you could change this
  // if you wanted to.
  if(p == 0) {
    swoopy_sounds = !swoopy_sounds;
  } else if(p == 1) {
    task_cycle = !task_cycle;
  }
  
  
}


// this is a callback function for when clyde first detects a new touch to its leg
// -- we do not use it in this example though.
void clydeDetected(uint8_t l) {

  //Serial << "Callback- Detected!" << l << endl;
  
}

