// here is the callback method that is called whenever the clydedev library detects that
// clyde's eye has been pressed.
void clydeEyePressed() {
 
  //Serial << "Callback- Eye Pressed!" << endl;
  
}


// here is the callback method that is called whenever the clydedev library detects that
// the touch on the eye has been released
void clydeEyeReleased() {
 
  //Serial << "Callback- Eye Released!" << endl;
  
  playTone(500, 100);
  delay(100);
  playTone(500, 100);
  delay(100);
  
  // toggle if clyde is playing sounds or not
  carolling = !carolling;
 
}


