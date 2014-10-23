// this function is called whenever we want clyde to sing a song. it goes to the
// next song each time we call it, so you will not become bored by listening to 
// the exact same song each time.
void singSong() {
  
  // call the specific song for a certain number
  if(current_song == 0) {
    heeHeeHooSong();
  } else if(current_song == 1) {
    sillySong();
  } else if(current_song == 2) {
    twinkleSong();
  }
  
  current_song++;
  if(current_song >= num_songs) current_song = 0; // reset when we hit the max number of songs
  
}


// sort of a funny song. i'm not a musician, so you would probably get better music
// from a chimpanzee. it's the same template for the next two functions as well.
void heeHeeHooSong() {

  uint8_t length = 16; // number of notes
  char notes[] = "ceg gfa dac efg"; // space represents a rest
  uint8_t beats[] = { 4, 1, 1, 1, 4, 1, 1, 1, 4, 1, 1, 1, 4, 1, 1, 1 }; // how 'long' each of the notes is played
  uint16_t tempo = 300; // how fast it all goes
  
  for (int i = 0; i < length; i++) {
    playSong(notes[i], beats[i], tempo); // send the song to be played
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
 
  uint8_t length = 15;
  char notes[] = "ccggaagffeeddc ";
  uint8_t beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
  uint16_t tempo = 300;
  
  for (int i = 0; i < length; i++) {
    playSong(notes[i], beats[i], tempo);
  }
  
}


// given the note, beat, and tempo, then play a note from this info
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
 
   if(peakToPeak >= 50) { // if there is volume, then cycle the hue
     cycleHue(); 
   } else {
     last_silence = millis(); // otherwise set the time that we have heard nothing, just silence
   }
   
   // check to see how long ago we have heard nothing. it makes sense because this would be a short 
   // time if there has been no sound (see above where last_silence is set), or a long time if there
   // has indeed been sound. the idea is that we want to check if it is actually a song playing, 
   // rather than just someone talking for a small moment in time.
   if(millis()-last_silence >= 500) { 
     
     if(millis()-last_added_humm >= 1000) { // clyde will 'humm' every 1 second
       
       uint16_t hoot = (int)random(0, 800);
       
       if(headsOrTails()) { // play a few tones...
       
         playTone(hoot, 50);
         delay(50);
         playTone(hoot+200, 50);
         delay(50);
         playTone(hoot+400, 50);
         
       } else { // or play a 'swoop' tone
         
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

