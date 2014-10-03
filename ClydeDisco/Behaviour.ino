void cycleHue() {
  
  if(hue_dir) {
    
    if(hue_num >= 360.0) {
      hue_num = 0.0;
    } else {
      hue_num+=5.0;
    }
    
  } else {
    
    if(hue_num <= 0.0) {
      hue_num = 360.0;
    } else {
      hue_num-=5.0;
    }
    
  }
  
  clyde.setEyeHSI(hue_num, 1.0, intensity);
  
}

void cycleLight() {

  if(light_num >= 155 && light_dir == true) {
    light_dir = false;
  } else if(light_num <= 50 && light_dir == false) {
    light_dir = true;
  }
  
  if(light_dir) {
   light_num+=10; 
  } else {
    light_num-=10;
  }
  
  clyde.setLight(light_num);
  
}

void cycleRGB(uint8_t c) {
  
  
  uint16_t val = clyde.current_colour[c];
    
  if(val >= 255) {
    
    if(c == 0) {
      red_dir = false;
    } else if(c == 1) {
      green_dir = false;
    } else if(c == 2) {
      blue_dir = false;
    }
    
  } else if(val <= 0) {
    
    if(c == 0) {
      red_dir = true;
    } else if(c == 1) {
      green_dir = true;
    } else if(c == 2) {
      blue_dir = true;
    }
    
  }
  
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


boolean headsOrTails() {

  if((int)random(0, 2) == 0) {
    return true;
  }
  
  return false;
  
}

