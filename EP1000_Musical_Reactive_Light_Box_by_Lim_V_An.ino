#include <Adafruit_NeoPixel.h>
#ifdef AVR
#include <avr/power.h>
#endif

int outputValue=0;
int rememberOutputValue;
int randNumber; // random number to generate random RGB colours on neopixel
int counter = 0; //counter initialised to 0
int loopCounter = 0; // loopcounter initialised to 0
int brightness=0; // potentiometer for brightness initialised to 0
int value = 50; // initialising neopixel brightness value 

const int buzzerPin = 2;    // initialising buzzer connected to pin 2
const int songLength = 24;  // number of notes of startup song is set


// Notes is an array of text characters corresponding to the notes in the startup song. A space represents a rest (no tone)

char notes[songLength] = {
  'g', 'g',' ', 'g',' ', 'g',' ', ' ', 'G',' ','D', 'D', ' ', 's', ' ','s',' ', 's',' ', ' ', 's',' ', 'D', ' '}; 

// beats[] is an array of values for each note. A "1" represents a quarter-note, 
// "2" a half-note, and "4" a quarter-note.
// rests (spaces) have a length as well.

double beats[songLength] = {
  0.25, 2.75, 0.25, 2.75, 0.25, 2.75, 0.25, 1.0, 2.0, 0.25, 0.3, 3.0, 1.0, 2.75, 0.25, 2.75, 0.25, 2.75, 0.25, 1.0, 2.0, 0.25, 3.0, 1.0};

int tempo = 100;  // The tempo sets the song BPM (beats per second).


#define PIN            6 // Arduino pin number 
#define NUMPIXELS      8 // number of pixels in strip

// Parameter 3 = pixel type flags, added together:
// NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
// NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); 

int buttonpressed = 0;  // initialising the button to be not pressed
const int button = 3;  // push button is connected to digital pin 3


void setup() {

  pinMode(buzzerPin, OUTPUT);  // sets the  buzzer pin as an OUTPUT
  pinMode(button, INPUT_PULLUP);  // sets the button as an input while turning on the pull-up resistor
  
  // setup code here runs once:
  pixels.begin();  // INITIALIZE NeoPixel strip object
  pixels.show();  // Send the updated pixel colors to the hardware
  
  startup_animation();  //runs the startup song and light animation 
  
  randomSeed(analogRead(0));
  Serial.begin(115200);
  
}

// A value 0 to 255 is inputed to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}


void loop() { 
// main code here runs repeatedly:
 
 lights_show();  // runs the lights_show function
 }

 void lights_show(){  // main function; for the neopixel lighting display 
   
 brightness = analogRead (A1);  //the potentiometer analog value is brightness data read from analog pin 1
 value = map(brightness, 0, 1024, 0, 255);  // maps the brightness data from one range to another
  //Function prototype: map(value, fromLow, fromHigh, toLow, toHigh)
  //parameters:
  //value: data to be mapped
  //fromLow: lower limit of the current range
  //formHigh: upper limit of the current range
  //toLow: lower limit of the target range
  //toHigh: upper limit of the target range
 
  pixels.setBrightness(value);  // sets the neopixels brightness value as the remapped data from above 
   int sensorValue;  //initialising the sound sensor data
   counter = 0;  //initialising counter to 0

   if (digitalRead(button) == LOW) //button is pressed AND this is the first digitalRead() that the button is pressed
  {
    while (digitalRead(button) == LOW)
    {
      // do nothing if button has not been pressed
    }
    delay(100);  // 0.1 second delay to "de-bounce" the circuit
    buttonpressed = buttonpressed + 1;  //declaring the buttonpressed value as itself with an increase of 1 per time of button pressed
    //if button is pressed more than 3 times, buttonpressed value is brought back to 0 to restart the increment of times of button pressed
    if (buttonpressed > 3) 
    {
      buttonpressed = 0;
    }
  } 
   
    //Serial.println(buttonpressed);  // uncomment for debuggin of push button
  if (buttonpressed == 0)  
  // if button is not pressed (light mode 1 [default]: normal sound sensitivity [light blinks/changes color when mid-volume sound is detected; 
  //will not work if sound is too soft])
  {
  for (int i=0; i < 100; i++){   
    sensorValue = analogRead(A0);   // sensorValue is the analog data read from pin 0 connected to sound sensor module(s)
    if(sensorValue > 630) counter++;  // increments the counter if sound sensor reads sound above 630 (loudness)
   }
  
   // Serial.println(sensorValue);  // uncomment for debugging of sound sensor
  if(map(counter, 10, 60, 80, 80) > outputValue)outputValue = map(counter, 00, 40, 0, 40);  // maps the counter value from one range to another  
  //Function prototype: map(value, fromLow, fromHigh, toLow, toHigh)
  //parameters:
  //value: data to be mapped
  //fromLow: lower limit of the current range
  //formHigh: upper limit of the current range
  //toLow: lower limit of the target range
  //toHigh: upper limit of the target range
  
  else if(loopCounter %2 == 0)outputValue-=1;

  // specifying a range of 0 - 60 for outputValue
  if(outputValue < 0) outputValue = 0;  
  if(outputValue > 60) outputValue = 60;

  if(loopCounter % 100 == 0)randNumber = random(255);  //generates random number from 0 to 255 for the RGB colour value
  loopCounter++;  //increments the loopCounter by 1

 for(int i=0;i<=NUMPIXELS;i++){  // For each pixel in the strip...
    // pixels.setPixelColor() takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0, 255));  // Set pixel's color as none
  } 
     if(rememberOutputValue != outputValue){
     for(int i=60;i > (60-outputValue) || (outputValue == 20 && i == 0);i--)
     {
     pixels.setPixelColor(i, Wheel((randNumber) & 255));  // Set pixel's color as a random color
     }
     pixels.show();  // Updates pixels to match
     }
  }
     
  else if (buttonpressed == 1)  // if button is pressed (light mode 2)
    gosoft();  // calls the gosoft() function
    
  else if (buttonpressed == 2)  // if button is pressed twice (light mode 3)
    gohard();  // calls the gohard() function
    
  else  // if button is pressed thrice (mode 4)
    goharder();  // calls the goharder() function
 
 rememberOutputValue = outputValue;  // the pixels color will remain the same when music stops
 }

void gosoft() {  
  // function for mode 2: low sound sensitivity (light only blinks/changes color when a relatively louder sound is detected, i.e. the significant parts of a song

   int sensorValue;  //initialising the sound sensor data
   counter = 0;  //initialising counter to 0
   
  for (int i=0; i < 100; i++){   
    sensorValue = analogRead(A0);   // sensorValue is the analog data read from pin 0 connected to sound sensor module(s)
    if(sensorValue > 78) counter++;  // increments the counter if sound sensor reads sound above 78 (loudness)
   }
  
   // Serial.println(sensorValue);  // uncomment for debugging of sound sensor
    if(map(counter, 10, 60, 80, 80) > outputValue)outputValue = map(counter, 00, 40, 0, 40);  // maps the counter value from one range to another  
  //Function prototype: map(value, fromLow, fromHigh, toLow, toHigh)
  //parameters:
  //value: data to be mapped
  //fromLow: lower limit of the current range
  //formHigh: upper limit of the current range
  //toLow: lower limit of the target range
  //toHigh: upper limit of the target range
  
  else if(loopCounter %2 == 0)outputValue-=1;

  // specifying a range of 0 - 60 for outputValue
  if(outputValue < 0) outputValue = 0;
  if(outputValue > 60) outputValue = 60;

  if(loopCounter % 100 == 0)randNumber = random(255);  //generates random number from 0 to 255 for the RGB colour value
  loopCounter++;  //increments the loopCounter by 1

  for(int i=0;i<=NUMPIXELS;i++){  // For each pixel in the strip...
    // pixels.setPixelColor() takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0, 255));  // Set pixel's color as none
  } 
   if(rememberOutputValue != outputValue){    
  for(int i=60;i > (60-outputValue) || (outputValue == 20 && i == 0);i--)
  {
    pixels.setPixelColor(i, Wheel((randNumber) & 255));  // Set pixel's color as a random color
    }
     pixels.show();  // Updates pixels to match
    }
}

 void gohard(){  
 // function for mode 3: dimming light mode (light dies down when sound is detected and lits back up in a sine wave-like fashion altogether when sound trails off)
   
   int sensorValue;  //initialising the sound sensor data
   counter = 0;  //initialising counter to 0
   
  for (int i=0; i < 100; i++){   
    sensorValue = analogRead(A0);   // sensorValue is the analog data read from pin 0 connected to sound sensor module(s)
    if(sensorValue > 420) counter++;  // increments the counter if sound sensor reads sound above 78 (loudness)
   }
  
    // Serial.println(sensorValue);  // uncomment for debugging of sound sensor
  if(map(counter, 10, 60, 80, 80) > outputValue)outputValue = map(counter, 00, 40, 0, 40);  // maps the counter value from one range to another  
  //Function prototype: map(value, fromLow, fromHigh, toLow, toHigh)
  //parameters:
  //value: data to be mapped
  //fromLow: lower limit of the current range
  //formHigh: upper limit of the current range
  //toLow: lower limit of the target range
  //toHigh: upper limit of the target range
  
  else if(loopCounter %2 == 0)outputValue-=1;

  // specifying a range of 0 - 60 for outputValue
  if(outputValue < 0) outputValue = 0;
  if(outputValue > 60) outputValue = 60;

  if(loopCounter % 100 == 0)randNumber = random(255);  //generates random number from 0 to 255 for the RGB colour value
  loopCounter++;  //increments the loopCounter by 1

 for(int i=0;i<=NUMPIXELS;i++){  // For each pixel in the strip...
    // pixels.setPixelColor() takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0, 255));  // Set pixel's color as none
  } 
  
     if(rememberOutputValue != outputValue){
     for(int i=60;i > (60-outputValue) || (outputValue == 20 && i == 0);i--){
     for(int j=0; j<256; j++)  // cycle all 256 colors in the wheel
     { 
     pixels.setPixelColor(i, Wheel((randNumber + j) & 255));
     }
     }
     pixels.show();  // Updates pixels to match
     }
}

void goharder(){  // function for mode 4: lightning flash mode (light stays off but flashes on when loud sound is detected)
   
   int sensorValue;  //initialising the sound sensor data
   counter = 0;  //initialising counter to 0
   
   for (int i=0; i < 100; i++){   
    sensorValue = analogRead(A0);   // sensorValue is the analog data read from pin 0 connected to sound sensor module(s)
    if(sensorValue > 630) counter++;  // increments the counter if sound sensor reads sound above 630 (loudness)
   }
  
  // Serial.println(sensorValue);  // uncomment for debugging of sound sensor
 if(map(counter, 10, 60, 80, 80) > outputValue)outputValue = map(counter, 00, 40, 0, 40);  // maps the counter value from one range to another  
  //Function prototype: map(value, fromLow, fromHigh, toLow, toHigh)
  //parameters:
  //value: data to be mapped
  //fromLow: lower limit of the current range
  //formHigh: upper limit of the current range
  //toLow: lower limit of the target range
  //toHigh: upper limit of the target range
  
  else if(loopCounter %2 == 0)outputValue-=1;

  // specifying a range of 0 - 60 for outputValue
  if(outputValue < 0) outputValue = 0;
  if(outputValue > 60) outputValue = 60;

  if(loopCounter % 100 == 0)randNumber = random(255);  //generates random number from 0 to 255 for the RGB colour value
  loopCounter++;  //increments the loopCounter by 1

  for(int i=0;i<=NUMPIXELS;i++){  // For each pixel in the strip...
    // pixels.setPixelColor() takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0, 255));  // Set pixel's color as none
  } 
  
    if(rememberOutputValue != outputValue){
    for(int i=60;i > (60-outputValue) || (outputValue == 20 && i == 0);i--)
    {
    pixels.setPixelColor(i, Wheel((randNumber) & 255));  // Set pixel's color as a random color
    }
     pixels.show();  // Updates pixels to match
    }
}
 
int frequency(char note) 
{
  int i;
  const int numNotes = 4;  // number of notes we're storing
  char names[numNotes] = { 
   'g', 'G', 'D', 's'   };
  double frequencies[numNotes] = {
    97.99886, 195.9977, 146.8324, 155.5635  };

  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.

  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      return(frequencies[i]);     // Yes! Return the frequency and exit function.
    }
  }
  return(0);  // We looked through everything and didn't find it,
  // but we still need to return a value, so return 0.
}

void startup_animation(){  // funnction for startup song and light display
  
  uint16_t i, j;
  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(6);
  }
  
 int x, duration; 

  for (x = 0; x < songLength; x++) // for loop is used to index through the arrays
  {
    duration = beats[x] * tempo;  // length of note/rest in ms

    if (notes[x] == ' ')          // is this a rest? 
      delay(duration);            // then pause for a moment
    else                          // otherwise, play the note
    {
      tone(buzzerPin, frequency(notes[x]), duration);
      delay(duration);            // wait for tone to finish
    }
    delay(tempo/10);              // brief pause between notes
  }
}
