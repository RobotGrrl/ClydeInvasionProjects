// this is a useful helper function that is like a coin toss. it either gives a
// true or false. in the code, it is used to help 'enhance' behaviours, by
// keeping it fresh and not repeating the same exact thing each time.
boolean headsOrTails() {

  if((int)random(0, 2) == 0) {
    return true;
  }
  
  return false;
  
}


// this helper functions is from the arduino tone tutorials: 
// http://www.arduino.cc/en/Tutorial/melody
void playTone(int tone, int duration) {
  
  if(!PLAY_SOUND) return;
  
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(spkr, HIGH);
    delayMicroseconds(tone);
    digitalWrite(spkr, LOW);
    delayMicroseconds(tone);
  }
	
}

