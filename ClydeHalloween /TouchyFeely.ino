
void clydeTouched(uint8_t l) {
 
  if(l == 0) {
    l = 3;
  } else if(l == 1) {
    l = 5;
  }
  
  Serial << "Callback- Touched! " << l << endl;
  
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


void clydeReleased(uint8_t l) {
 
  if(l == 0) {
    l = 3;
  } else if(l == 1) {
    l = 5;
  }
  
  //Serial << "Callback- Released!" << l << endl;
  
  switch(l) {
    case 3: // trick
      
      for(uint8_t i=0; i<5; i++) {
        playTone(400, 40);
        playTone(500, 40);
      }
      
      for(uint8_t j=0; j<2; j++) {
        for(uint8_t i=0; i<72; i++) {
          clyde.setEyeHSI((float)(i*5), 1.0, 1.0);
          delay(5);
        }
      }
      
      for(uint8_t i=0; i<5; i++) {
        playTone(1000, 40);
        playTone(300, 40);
      }
      
      for(uint8_t j=0; j<2; j++) {
        for(uint8_t i=72; i>0; i--) {
          clyde.setEyeHSI((float)(i*5), 1.0, 1.0);
          delay(5);
        }
      }
      
      blink_block = false;
      
    break;
    case 5: // treat
      
      playTone(500, 100);
      playTone(800, 100);
      playTone(1100, 100);
      
      for(uint8_t i=0; i<12; i++) {
        
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
        
        clyde.setEyeHSI(random(0, 360), 1.0, 1.0);
        
      }
      
      clyde.setLight(200);
      delay(3000);
      
      blink_block = false;
      
    break;
  }
 
  clyde.setLight(light_num);
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  
}


void clydeDetected(uint8_t l) {

  //Serial << "Callback- Detected!" << l << endl;
  
}




