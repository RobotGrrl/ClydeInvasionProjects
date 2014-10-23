// cycle hue changes the hue of clyde's eye.
   
// we increase or decrease the hue by 5 degrees each time, as to make the change 
// visible over a short period of time.
   
// the direction that the hue is going is determined by hue_dir.
   
// finally, the intensity of the eye colour change is determined by intensity.
   
// this is supposed to be a helper function for whenever you want to have a
// rainbow of colours splash on clyde's eye!
void cycleHue() {
  
  if(hue_dir) {
    
    if(hue_num >= 360.0) {
      hue_num = 0.0; // reset it if it hits 360 degrees
    } else {
      hue_num+=5.0;
    }
    
  } else {
    
    if(hue_num <= 0.0) {
      hue_num = 360.0; // reset it if it hits 0 degrees
    } else {
      hue_num-=5.0;
    }
    
  }
  
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  
}


// cycle light is really similar to cycle hue, but instead of the eye, it is the
// task light, or the big white light below clyde's body.
   
// each time cycleLight() is called, it increases or decreases the light by 10
// (arbitrary units).
   
// if light_dir is true, then this means the light is getting brighter. if 
// light_dir is false, then this means the light is getting darker.
   
// you can continuously call cycleLight() to have a sleeping / breathing effect.
void cycleLight() {

  if(light_num >= 155 && light_dir == true) { // if it hits the max brightness, switch directions
    light_dir = false;
  } else if(light_num <= 50 && light_dir == false) { // if it hits the min brightness, switch directions
    light_dir = true;
  }
  
  if(light_dir) {
   light_num+=10; 
  } else {
    light_num-=10;
  }
  
  clyde.setLight(light_num);
  
}


// cycle RGB is again, similar to cycle HSI. except that with RGB, it cycles
// whichever individual colour you pass to it.
  
// 0: red, 1: green, 2: blue
   
// the direction of the cycle automatically flips when it hits 255 or 0. 
void cycleRGB(uint8_t c) {
  
  
  uint16_t val = clyde.current_colour[c];
  
  if(val >= 255) { // change directions if it hits the max boundary
    
    if(c == 0) {
      red_dir = false;
    } else if(c == 1) {
      green_dir = false;
    } else if(c == 2) {
      blue_dir = false;
    }
    
  } else if(val <= 0) { // change directions if it hits the min boundary
    
    if(c == 0) {
      red_dir = true;
    } else if(c == 1) {
      green_dir = true;
    } else if(c == 2) {
      blue_dir = true;
    }
    
  }
  
  
  // in this block of code, we are checking what colour and what direction we have.
  // based on this, we incremement or decrement the specific led
  
  if(c == 0) {
    
    if(red_dir) {
      clyde.setEyeRGB(val+1, clyde.current_colour[1], clyde.current_colour[2]);
    } else {
      clyde.setEyeRGB(val-1, clyde.current_colour[1], clyde.current_colour[2]);
    }
    
  } else if(c == 1) {
    
    if(green_dir) {
      clyde.setEyeRGB(clyde.current_colour[0], val+1, clyde.current_colour[2]);
    } else {
      clyde.setEyeRGB(clyde.current_colour[0], val-1, clyde.current_colour[2]);
    }
    
  } else if(c == 2) {
    
    if(blue_dir) {
      clyde.setEyeRGB(clyde.current_colour[0], clyde.current_colour[1], val+1);
    } else {
      clyde.setEyeRGB(clyde.current_colour[0], clyde.current_colour[1], val-1);
    }
    
  }
    
  
}


// this is a useful helper function that is like a coin toss. it either gives a
// true or false. in the code, it is used to help 'enhance' behaviours, by
// keeping it fresh and not repeating the same exact thing each time.
boolean headsOrTails() {

  if((int)random(0, 2) == 0) {
    return true;
  }
  
  return false;
  
}

