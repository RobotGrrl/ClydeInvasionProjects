/*
 * Clyde Cheerlights App
 * ----------------------
 *
 * Let Clyde display cheerlights and participate in the global
 * interconnectedness of LED lights!
 *
 * This Processing sketch checks the cheerlights ThingSpeak feed
 * for the last colour every 5 seconds. The current colour is then
 * sent to Clyde through Serial! Clyde then updates its eye RGB LEDs.
 *
 * Make sure you set your proper serial port in the Arduino tab, in
 * the variable clyde_serial.
 *
 * Upload the corresponding Arduino code to Clyde (so that it can
 * 'listen' for the specific colours), run this Processing sketch,
 * and sit back and relax to enjoy the show!
 *
 * To participate in changing the cheerlights, you just tweet
 * @cheerlights with the colour you want. Pretty cool!
 *
 * Written by Erin RobotGrrl
 * --> http://robotgrrl.com
 *
 * For more info about Clyde:
 * --> http://fabule.com/clyde
 *
 * Learn more about Cheerlights:
 * --> http://cheerlights.com
 *
 * December 20, 2014
 */


import processing.serial.*;

static final boolean DEBUG = false;

//String URL = "http://api.thingspeak.com/channels/1417/feed.json";
String URL = "http://api.thingspeak.com/channels/1417/field/1/last.json";
int FREQ = 5; // refresh frequency (in seconds)

String[] all_colour_names = {
  "red",
  "green",
  "blue",
  "cyan",
  "white",
  "warmwhite",
  "purple",
  "magenta",
  "yellow",
  "orange",
  "pink",
  "oldlace" };
  
int[][] all_colour_rgb = {
  {230, 51, 72},
  {15, 171, 19},
  {95, 144, 255},
  {67, 255, 240},
  {255, 255, 255},
  {250, 249, 202},
  {167, 72, 255},
  {234, 17, 226},
  {252, 233, 26},
  {255, 164, 45},
  {254, 129, 206},
  {192, 192, 192}
};

int r = 255;
int g = 255;
int b = 255;

int sec_0 = 88; // previous second
int sec; // current second
int sec_count = 0; // needed to count the seconds past 60

PImage brrdy;

int last_heartbeat = 0;
int colour_index = 99;

DisposeHandler dh;

void setup() {
  
  noStroke();
  colorMode(RGB);
  size(200, 200); 
  
  dh = new DisposeHandler(this);

  connectArduino();
  
  // if you don't want to force the connection, then comment this part out.
  while(!connected) {
    connectArduino();
    delay(1000);
  }
  
  refreshStream();
  
  brrdy = loadImage("pixel_brrd.png");
  
}


void draw() {
  
  background( color(r, g, b) );
  //image(brrdy, 0, 0);
  
  sec = second();

  // check if it is time to refresh yet
  if(sec_count%FREQ == 0 && sec_0 != sec && sec_count != 0) {
    println("\n\nding!");
    sec_0 = sec;
    sec_count = 0;
    
    refreshStream();

    // checking here to see if we have not received a heartbeat lately
    if(millis()-last_heartbeat >= 1000 * 91) {
      
      println("Have not received a heartbeat, going to reconnect");
      
      arduino.clear();
      arduino.stop();
      connected = false;
      
      // reconnect vigorously
      while(!connected) {
        println("Trying to reconnect");
        connectArduino();
        delay(1000);
      }
      
    }
    
  } else {
    // print out the second count
    if (sec != sec_0) {
      print(sec_count + " ");
      sec_0 = sec;
      sec_count++;
    }
  }
  
  readData();
  
}


// refreshing the thingspeak feed, extract all the relevant info from
// the json file
void refreshStream() {
  
  String result = "";
  JSONObject root = loadJSONObject(URL);

  result = root.getString("field1");
  
  println("Latest colour: " + result);
  
  colour_index = 99;
  for(int i=0; i<all_colour_names.length; i++) {
    if(result.equals(all_colour_names[i])) {
      colour_index = i;
      break;
    }
  }
  
  if(colour_index == 99) {
    println("Did not find that colour");
    return; 
  }
  
  r = all_colour_rgb[colour_index][0];
  g = all_colour_rgb[colour_index][1];
  b = all_colour_rgb[colour_index][2];
   
  println("LED R: " + r + " G: " + g + " B: " + b);
  
  sendColour();
  
}


// sending the colour info to clyde through the serial port
void sendColour() {

  println("Sending colour data... ");
 
  String s = (colour_index+1) + "";
  
  if(connected) {
    println(s);
    arduino.write(s);
    delay(50);
  }
  
  println("Done!");
  
}



// snippet of code credit to this forum post:
// http://forum.processing.org/one/topic/run-code-on-exit.html

public class DisposeHandler {
  
  DisposeHandler(PApplet pa) {
    pa.registerDispose(this);
  }
 
  public void dispose() {
    println("In dispose");
    if(connected) {
      arduino.clear();
      arduino.stop();
    }
  }
  
}


