// here is the callback method that is called whenever the clydedev library detects that
// clyde's eye has been pressed.
void clydeEyePressed() {
 
 Serial << "Callback- Eye Pressed!" << endl;
  
}


// here is the callback method that is called whenever the clydedev library detects that
// the touch on the eye has been released
void clydeEyeReleased() {
 
  Serial << "Callback- Eye Released!" << endl;
  
  // play a weird / creepy sound thingy. lol.
  for(uint16_t i=800; i<1000; i+=50) {
    for(uint8_t j=0; j<5; j++) {
      playTone(i, 20);
      delay(50);
    }
  }
 
}


