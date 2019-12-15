#include <Arduino.h>
#include <SPI.h>
#include <FastLED.h>

int buff [32];
volatile byte indx;
volatile boolean newMessage;

//0 54
//55 97
//98 145
//146 188
//189 228
//229 274

#define TURNOFFLEDS 0 
#define SHOWNAME    1
#define HINT        2

#define NUM_LEDS 274
#define BRIGHTNESS 128

#define DATA_PIN A0

CRGB leds[NUM_LEDS];
const int ledAmount = NUM_LEDS;
const int nameLedAmount[6] = {54, 42, 47, 42, 39, 45};
const int nameLedBegin[6] = {0, 55, 98, 146, 189, 229}; //Do we need this? It's current nameLedAmount + the previous ones?
const int writeSpeed = 10;

void ProcessMessage();
void ShowName(int name, int speed);
void TurnOffAllLeds();

void setup() {
    Serial.begin (115200);
    Serial.println("Names: LED SLAVE..");
   	delay(2000);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    delay(1000);
    pinMode(MISO, OUTPUT); // have to send on master in so it set as output
    SPCR |= _BV(SPE); // turn on SPI in slave mode
    indx = 0; 
    newMessage = false;
    SPI.attachInterrupt(); 
    TurnOffAllLeds();
}

void loop() {
     
  if (newMessage) {    
    ProcessMessage();
    indx = 0;    
    newMessage = false;     
  } 
}

void ProcessMessage()
{
    Serial.print("Processing messsage: ");    
    for(int i = 0; i < indx - 1; i++)
    {
      Serial.print(buff[i]);
      Serial.print(", ");
    }
    Serial.println();
    int command = buff[0];
    int value = buff[1];
    switch (command)
    {
    case TURNOFFLEDS:
      TurnOffAllLeds();
      break;
    case SHOWNAME:
      ShowName(value, 50);
      FastLED.show();
      break;
    case HINT:
      leds[value] = CRGB::Green;
      FastLED.show();
      break;    
    default:
      Serial.println("Unknown Command: " + String(command));
      break;
    }
}

void ShowName(int name, int speed)
{
    for (int i = nameLedBegin[name]; i < nameLedBegin[name] + nameLedAmount[name]; i++)
    {
        for (int b = 0; b < BRIGHTNESS; b += speed)
        {
            leds[i].green = b;
            FastLED.show();
        }   
    }   
}

void TurnOffAllLeds()
{
  for(int i = 0; i < NUM_LEDS; i++) {       
      leds[i] = CRGB::Black;
  }  
  FastLED.show();
}

ISR (SPI_STC_vect) {
  int c = SPDR; // read byte from SPI Data Register
  if (indx < sizeof buff) {
    buff [indx++] = c; 
    if ( c == '\n') 
        newMessage = true;
  }
}