#include <SPI.h>
#include <FastLED.h>

int buff [255];
volatile byte indx;
volatile boolean process;

#define NUM_LEDS 10
#define DATA_PIN A0

#define SETLED 0
#define SHOWLEDS 1

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin (115200);
  Serial.println("SPI PLEASE");
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  pinMode(MISO, OUTPUT); // have to send on master in so it set as output
  SPCR |= _BV(SPE); // turn on SPI in slave mode
  indx = 0; 
  process = false;
  SPI.attachInterrupt(); 
}

void loop() {
  if (process) {
    process = false;
    indx = 0;
    processRequest();     
  }
}

void processRequest()
{
   int type     = buff[0];
   int ledIndex = buff[1];
   int value_r  = buff[2];
   int value_g  = buff[3];
   int value_b  = buff[4];
   
   switch(type)
   {
      case SETLED: //LedInfo
        leds[ledIndex].red = value_r;
        leds[ledIndex].green = value_g;
        leds[ledIndex].blue = value_b;
        break;
      case SHOWLEDS: //Show
        FastLED.show();
        break;
   }
}

ISR (SPI_STC_vect) {
//  Serial.println(indx);
  byte c = SPDR; // read byte from SPI Data Register
  if (indx < sizeof buff) {
    buff [indx++] = c; 
    if ( c == '\n') 
        process = true;
  }
}
