// this is the callback function that is called whenever one of clyde's legs is touched
void clydeTouched(uint8_t l) {
 
  // this is just a fix based on our touchy feely pin configurations
  if(l == 0) {
    l = 3;
  } else if(l == 1) {
    l = 5;
  }
  
  Serial << "Callback- Touched! " << l << endl;
  
  // whenever the trick or treat leg is touched, we will turn on the blink_block
  // to prevent interruption with the ambient behaviour, and also set the eye to
  // a new colour.
  
  // the remainder of the behaviour happens when the leg is released, so check out
  // the clydeReleased callback method.
  
  switch(l) {
    case 3: // trick
      
      blink_block = true;
      
      playTone(300, 80);
      clyde.setEyeHSI(random(0, 180), 1.0, 1.0);
      
      delay(100);
      
    break;
    case 5: // treat
      
      playTone(900, 80);
      clyde.setEyeHSI(random(180, 360), 1.0, 1.0);
      
      blink_block = true;
      
    break;
 }
  
}


// this is a callback function for the touchy feely module. it is called whenever one of clyde's
// legs is released
void clydeReleased(uint8_t l) {
 
  // this is just a fix based on our touchy feely pin configurations
  if(l == 0) {
    l = 3;
  } else if(l == 1) {
    l = 5;
  }
  
  //Serial << "Callback- Released!" << l << endl;
  
  switch(l) {
    case 3: // trick
      
      // play some music
      for(uint8_t i=0; i<5; i++) {
        playTone(400, 40);
        playTone(500, 40);
      }
      
      // rainbow cycle 2x!
      for(uint8_t j=0; j<2; j++) {
        for(uint8_t i=0; i<72; i++) {
          clyde.setEyeHSI((float)(i*5), 1.0, 1.0);
          delay(5);
        }
      }
      
      // beep some more, clyde
      for(uint8_t i=0; i<5; i++) {
        playTone(1000, 40);
        playTone(300, 40);
      }
      
      // another rainbow cycle 2x!
      for(uint8_t j=0; j<2; j++) {
        for(uint8_t i=72; i>0; i--) {
          clyde.setEyeHSI((float)(i*5), 1.0, 1.0);
          delay(5);
        }
      }
      
      // remember to remove the lock
      blink_block = false;
      
    break;
    case 5: // treat
      
      // special sounds
      playTone(500, 100);
      playTone(800, 100);
      playTone(1100, 100);
      
      for(uint8_t i=0; i<12; i++) {
        
        // sparkle the light
        if(headsOrTails()) {
          clyde.setLight(30);
        } else {
          clyde.setLight(200);
        }
        
        // play a sound every 3rd time
        if(i%3 == 0) {
          
          uint16_t hoot = (int)random(200, 1000);
        
          for(int i=hoot; i<hoot+500; i+=50) {
             playTone(i, 10);
           }
          
        } else {
          delay(100); 
        }
        
        // random eye colour each time
        clyde.setEyeHSI(random(0, 360), 1.0, 1.0);
        
      }
      
      // reveal the *CANDY* (an apple, lol)
      clyde.setLight(200);
      delay(3000);
      
      // remember to remove the lock
      blink_block = false;
      
    break;
  }
 
  // return everything to normal
  clyde.setLight(light_num);
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  
}


// this is a callback function for when clyde first detects a new touch to its leg
// -- we do not use it in this example though.
void clydeDetected(uint8_t l) {
  //Serial << "Callback- Detected!" << l << endl;
}




