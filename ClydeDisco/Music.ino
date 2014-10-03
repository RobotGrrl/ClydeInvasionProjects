void singSong() {
  
  if(current_song == 0) {
    heeHeeHooSong();
  } else if(current_song == 1) {
    sillySong();
  } else if(current_song == 2) {
    twinkleSong();
  }
  
  current_song++;
  if(current_song >= num_songs) current_song = 0;
  
}


void heeHeeHooSong() {

  uint8_t length = 16;
  char notes[] = "ceg gfa dac efg";
  uint8_t beats[] = { 4, 1, 1, 1, 4, 1, 1, 1, 4, 1, 1, 1, 4, 1, 1, 1 };
  uint16_t tempo = 300;
  
  for (int i = 0; i < length; i++) {
    playSong(notes[i], beats[i], tempo);
  }
  
}


void sillySong() {

  uint8_t length = 14;
  char notes[] = "Cbag cdef efef";
  uint8_t beats[] = { 4, 2, 1, 1, 1, 4, 2, 1, 1, 1, 1, 1, 1, 1 };
  uint16_t tempo = 300;
  
  for (int i = 0; i < length; i++) {
    playSong(notes[i], beats[i], tempo);
  }
  
}


void twinkleSong() {
 
  uint8_t length = 15; // the number of notes
  char notes[] = "ccggaagffeeddc "; // a space represents a rest
  uint8_t beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
  uint16_t tempo = 300;
  
  for (int i = 0; i < length; i++) {
    playSong(notes[i], beats[i], tempo);
  }
  
}


void playSong(char notes, uint8_t beats, uint16_t tempo) {
  
  if (notes == ' ') {
    delay(beats * tempo); // rest
  } else {
    playNote(notes, beats * tempo);
  }

  // pause between notes
  delay(tempo / 2); 
    
}


// this function is from adafruit
// https://learn.adafruit.com/adafruit-microphone-amplifier-breakout/measuring-sound-levels

void micSample() {
 
 unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(mic);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   //double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
 
   if(!task_cycle) clyde.setLight( (int)(peakToPeak / 4) );
 
   if(peakToPeak >= 50) {
     cycleHue(); 
   } else {
     last_silence = millis(); 
   }
   
   if(millis()-last_silence >= 500) {
     
     if(millis()-last_added_humm >= 1000) {
       
       uint16_t hoot = (int)random(0, 800);
       
       if(headsOrTails()) {
       
         playTone(hoot, 50);
         delay(50);
         playTone(hoot+200, 50);
         delay(50);
         playTone(hoot+400, 50);
         
       } else {
         
         for(int i=hoot; i<hoot+500; i+=50) {
           playTone(i, 10);
         }
         
         for(int i=hoot+500; i>hoot; i-=50) {
           playTone(i, 10);
         }
         
       }
       
       last_added_humm = millis();
     }
     
   }
 
   //Serial << (int)(peakToPeak / 4) << endl;
  
}


// these two helper functions are from the arduino tone
// tutorials: http://www.arduino.cc/en/Tutorial/melody

void playTone(int tone, int duration) {
	
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(spkr, HIGH);
    delayMicroseconds(tone);
    digitalWrite(spkr, LOW);
    delayMicroseconds(tone);
  }
	
}

void playNote(char note, int duration) {
  
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
  
}

