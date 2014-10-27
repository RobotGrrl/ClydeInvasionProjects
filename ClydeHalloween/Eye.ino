// here is the callback method that is called whenever the clydedev library detects that
// clyde's eye has been pressed.
void clydeEyePressed() {
 
 Serial << "Callback- Eye Pressed!" << endl;
 
 // whenever the eye is pressed, we will play an interesting tone.
 // to liven it up a bit, we change the tone pitch based on heads
 // or tails result.
 if(headsOrTails()) {
 
   for(int i=1000; i<1000+1000; i+=100) {
     playTone(i, 10);
   }
 
 } else {
   
   for(int i=100; i<100+1000; i+=100) {
     playTone(i, 10);
   }
   
 }
 
}


// here is the callback method that is called whenever the clydedev library detects that
// the touch on the eye has been released
void clydeEyeReleased() {
  //Serial << "Callback- Eye Released!" << endl;
}


