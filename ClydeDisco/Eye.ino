// here is the callback method that is called whenever the clydedev library detects that
// clyde's eye has been pressed.
void clydeEyePressed() {
 
 Serial << "Callback- Eye Pressed!" << endl;

 // if the eye was not pressed before (ie, it is a new press), then make sure the
 // starting colours that we will be using are the ones that clyde is currently
 // displaying 
 if(!currently_pressed) {
   start_r = clyde.current_colour[0];
   start_g = clyde.current_colour[1];
   start_b = clyde.current_colour[2];
   currently_pressed = true;
 }
 
 // we only want it to blink every 100ms or so
 if(millis()-last_blink >= 100) {
   
   uint8_t r = 0;
   uint8_t g = 0;
   uint8_t b = 0;
 
   // check if it will be going dim or bright and set the colours
   if(blink_on) {
     
     r = 0;
     g = 0;
     b = 0;
     
     // as long as the current colour is more than '20' (arbitrary value), then subtract
     // 20 from the colour to make it look more dim
     if(clyde.current_colour[0] > 20) r = clyde.current_colour[0]-20;
     if(clyde.current_colour[1] > 20) g = clyde.current_colour[1]-20;
     if(clyde.current_colour[2] > 20) b = clyde.current_colour[2]-20;
     
   } else {
     
     r = 255;
     g = 255;
     b = 255;
     
     // as long as the current colour is less than '255-20' (arbitrary value), then add
     // 20 to the colour to make it look more bright
     if(clyde.current_colour[0] < 255-20) r = clyde.current_colour[0]+20;
     if(clyde.current_colour[1] < 255-20) g = clyde.current_colour[1]+20;
     if(clyde.current_colour[2] < 255-20) b = clyde.current_colour[2]+20;
     
   }
   
   clyde.setEyeRGB(r, g, b);
   
   blink_on = !blink_on; // don't forget to trigger the led for next time!
   
   last_blink = millis();
   
 }
 
}


// here is the callback method that is called whenever the clydedev library detects that
// the touch on the eye has been released
void clydeEyeReleased() {
 
  Serial << "Callback- Eye Released!" << endl;

  // reset the eye to its starting colours   
  clyde.setEyeRGB(start_r, start_g, start_b);
  
  // and sing us a song too, because you're a robot and we get to make you do whatever we want! :p
  if(currently_pressed) {
    singSong(); 
  }
  
  currently_pressed = false;
  
}


