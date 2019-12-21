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
#define BRIGHTNESS  3
#define COLOR_R     10
#define COLOR_G     11
#define COLOR_B     12
#define WRITESPEED  13
#define ENDMESSAGE 255

#define NUM_LEDS 274


#define DATA_PIN A0
#define READYREADY_PIN A1

CRGB leds[NUM_LEDS];
const int ledAmount = NUM_LEDS;
const int nameLedAmount[6] = {54, 42, 47, 42, 39, 45};
const int nameLedBegin[6] = {0, 55, 98, 146, 189, 229}; //Do we need this? It's current nameLedAmount + the previous ones?

int writeSpeed = 10;
int brightness = 64;

void ProcessMessage();
void ShowName(uint8_t name, uint8_t speed);
void TurnOffAllLeds();
void ShowHint(uint8_t type);
void SetBrightness(uint8_t bright);
void SetWriteSpeed(uint8_t speed);

void setup() {
    Serial.begin (115200);
    Serial.println("Names: LED SLAVE..");
   	delay(2000);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(brightness);
    delay(1000);
    pinMode(MISO, OUTPUT); // have to send on master in so it set as output
    SPCR |= _BV(SPE); // turn on SPI in slave mode
    indx = 0; 
    newMessage = false;
    SPI.attachInterrupt(); 
    TurnOffAllLeds();
    pinMode(READYREADY_PIN, OUTPUT); // have to send on master in so it set as output
    digitalWrite(READYREADY_PIN, HIGH);
}

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 8;           // interval at which to blink (milliseconds)

void loop() {
  if (newMessage) {   
    digitalWrite(READYREADY_PIN, LOW);
    ProcessMessage();
    indx = 0;    
    newMessage = false;    
    digitalWrite(READYREADY_PIN, HIGH); 
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
      ShowName(value, writeSpeed);
      break;
    case HINT:
      ShowHint(value);
      break;  
    case BRIGHTNESS:
      SetBrightness(value);
      break; 
     case WRITESPEED:
      SetWriteSpeed(value);
      break;  
    default:
      Serial.println("Unknown Command: " + String(command));
      break;
    }
}

#define LODEWEIJCK 0
#define HENDRIK 1
#define CATHARINA 2
#define RUDOLF 3
#define BERNARD 4
#define CORNELIA 5
#define JOHANNES 6

#define MURDERER 7
#define SYMBOLS 8

void ShowHint(uint8_t type)
{
    if(type < MURDERER)
    {

    }else if(type == MURDERER)
    {

    }else if(type == SYMBOLS)
    {
      
    }
}

void SetBrightness(uint8_t bright)
{
    Serial.println("Setting Brightness");
    brightness = bright;
    FastLED.setBrightness(brightness);
    FastLED.show();
}

void SetWriteSpeed(uint8_t speed)
{
    Serial.println("Setting WriteSpeed");
    writeSpeed = speed;
}

void ShowName(uint8_t name, uint8_t speed)
{
  Serial.println("Showing Name");
  for (int i = nameLedBegin[name]; i < nameLedBegin[name] + nameLedAmount[name]; i++)
    {
        for (int b = 0; b < brightness; b += speed)
        {
            leds[i].green = b;
            FastLED.show();
        }   
    }   
}

void TurnOffAllLeds()
{
  Serial.println("TurnOffAllLeds");
  for(int i = 0; i < NUM_LEDS; i++) {       
      leds[i] = CRGB::Black;
  }  
  FastLED.show();
}

ISR (SPI_STC_vect) {
  int c = SPDR; // read byte from SPI Data Register
  if (indx < sizeof buff) {
    buff [indx++] = c; 
    if ( c == ENDMESSAGE) 
        newMessage = true;
  }
}