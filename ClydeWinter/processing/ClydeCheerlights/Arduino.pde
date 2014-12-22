// -- ARDUINO -- //
String clyde_serial = "/dev/tty.usbmodemfd14421";
Serial arduino;
int port = 99;
boolean connected = false;

// -- PARSING -- //
int msgLen = 20;
int msgIndex = 0;
char[] msg = new char[msgLen];
boolean reading = false;
boolean completed = false;
int receivedValue = 0;


// let's connect to our arduino, on 9600 baud!
void connectArduino() {
 
  for (int i=0; i<Serial.list().length; i++) {
    println(Serial.list()[i]);
    if (Serial.list()[i].equals(clyde_serial)) {
    //if (Serial.list()[i].equals("/dev/tty.Bloopity-SPP")) {
      println("Found serial port!");
      port = i;
    }
  }
  
  if(port == 99) {
    println("Did not find serial port"); 
  }
  
  try {
    arduino = new Serial(this, Serial.list()[port], 9600);
    delay(500); // seriously
    connected = true;
  } catch(Exception e) {
    print(e.getMessage());
    println(e.toString());
    connected = false;
  }
  
  if(connected) {
    connected = true;
    println("Connected!");
  } else {
    connected = false;
    println("Not connected"); 
  }
  
}


// receiving data from the arduino. a heartbeat, for example! :)
void readData() {
 
  if(connected) {
    // let's read the data
    char c;
    
    while(arduino.available() > 0) {
      
      c = arduino.readChar();
      
      if(c == '~') {
        println("received heartbeat");
        last_heartbeat = millis();
      }
      
    }
    
  }
  
}

