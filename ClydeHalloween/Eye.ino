void clydeEyePressed() {
 
 Serial << "Callback- Eye Pressed!" << endl;
 
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

void clydeEyeReleased() {
 
  Serial << "Callback- Eye Released!" << endl;
 
}


