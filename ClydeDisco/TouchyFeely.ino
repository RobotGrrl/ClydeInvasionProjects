
void clydeTouched(uint8_t l) {
 
  Serial << "Callback- Touched! " << l << endl;
  
  switch(l) {
    case 0: {
      
      uint8_t leg_num = 1;
    
      if(millis() - last_touch_time[leg_num] >= pause_thresh) {
        
        hue_num = 0.0;
        clyde.setEyeHSI(hue_num, 1.0, intensity);
        
        playTone(800, touch_tone_duration);
        
        if(swoopy_sounds) {
          for(int i=800; i<800+1000; i+=100) {
            playTone(i, 10);
          } 
        }
        
        newLegPressed(leg_num);
        last_touch_time[leg_num] = millis();
      
      }
      
      legTouched[leg_num] = true;
    
    }
    break;
    case 1: {
    
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
    case 2: {
      
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
    case 5: {
    
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


void clydeReleased(uint8_t l) {
 
  //Serial << "Callback- Released!" << l << endl;
  
  uint8_t leg_num = 8;
  
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
  
  allReleased = true;
  for(int i=0; i<4; i++) {
    if(legTouched[i]) {
      allReleased = false;
      break;
    }
  }
  
  if(leg_num == 9) allReleased = false;
  
  if(allReleased) {
    
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


void newLegPressed(uint8_t l) {
  
  legPresses[3] = legPresses[2];
  legPresses[2] = legPresses[1];
  legPresses[1] = legPresses[0];
  legPresses[0] = l;
  
}


void checkLegPatterns() {
 
 for(int j=0; j<numPatterns; j++) {
   
   boolean match = true;
   
   for(int i=0; i<4; i++) {
   
     Serial << legPresses[3-i] << " vs " << legPattern[j][i] << endl;
     
     if(legPresses[3-i] != legPattern[j][i]) {
       match = false;
       break;
     }
   
   }
   
   if(match) patternMatched(j);
   
 }
  
}


void patternMatched(uint8_t p) {
  Serial << "the #" << p << " pattern matched" << endl;
  
  // celebration!
  
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
  
  
  if(p == 0) {
    swoopy_sounds = !swoopy_sounds;
  } else if(p == 1) {
    task_cycle = !task_cycle;
  }
  
  
}


void clydeDetected(uint8_t l) {

  //Serial << "Callback- Detected!" << l << endl;
  
}




