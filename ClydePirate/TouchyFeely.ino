// this is the callback function that is called whenever one of clyde's legs is touched
void clydeTouched(uint8_t l) {
 
  Serial << "Callback- Touched! " << l << endl;
  
  // based on the leg, set the different settings
  
  switch(l) {
    case 0: // front right
      dist_thresh = 10;
      dist_time = 3000;
      alarm_mode = 0;
    break;
    case 1: // front left
      dist_thresh = 10;
      dist_time = 3000;
      alarm_mode = 1;
    break;
    case 2: // back right
      dist_thresh = 18;
      dist_time = 5000;
      alarm_mode = 0;
    break;
    case 3: // back left
      dist_thresh = 18;
      dist_time = 5000;
      alarm_mode = 1;
    break;
  }
  
}


// this is a callback function for the touchy feely module. it is called whenever one of clyde's
// legs is released
void clydeReleased(uint8_t l) {
 
  Serial << "Callback- Released!" << l << endl;
  
  // blink the eye to signify that the settings have been set
  
  for(uint8_t i=0; i<8; i++) {
    clyde.setEyeHSI(0.0, 0.0, 1.0);
    delay(100);
    clyde.setEyeHSI(135.0, 1.0, 1.0);
    delay(100);
  }
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  
  // a little hack to stop the glitchies
  touchyfeely.reset();
  
}


// this is a callback function for when clyde first detects a new touch to its leg
void clydeDetected(uint8_t l) {

  //Serial << "Callback- Detected!" << l << endl;
  
}

