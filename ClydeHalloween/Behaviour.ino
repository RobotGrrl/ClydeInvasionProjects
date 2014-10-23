
boolean headsOrTails() {

  if((int)random(0, 2) == 0) {
    return true;
  }
  
  return false;
  
}


// this helper functions is from the arduino tone tutorials: 
// http://www.arduino.cc/en/Tutorial/melody

void playTone(int tone, int duration) {
	
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(spkr, HIGH);
    delayMicroseconds(tone);
    digitalWrite(spkr, LOW);
    delayMicroseconds(tone);
  }
	
}

