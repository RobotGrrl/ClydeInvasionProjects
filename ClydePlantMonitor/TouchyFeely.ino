// this is the callback function that is called whenever one of clyde's legs is touched
void clydeTouched(uint8_t l) {
 
  //Serial << "Callback- Touched! " << l << endl;
  
}


// this is a callback function for the touchy feely module. it is called whenever one of clyde's
// legs is released
void clydeReleased(uint8_t l) {
 
  //Serial << "Callback- Released!" << l << endl;
  
}


// this is a callback function for when clyde first detects a new touch to its leg
void clydeDetected(uint8_t l) {

  //Serial << "Callback- Detected!" << l << endl;
  
}

