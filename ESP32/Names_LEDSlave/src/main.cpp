#include <FastLED.h>

//0 54
//55 97
//98 145
//146 188
//189 228
//229 274

#define NUM_LEDS 274
#define BRIGHTNESS 255

#define DATA_PIN A0

CRGB leds[NUM_LEDS];
void setup() {
   	delay(2000);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
     leds[whiteLed] = CRGB::White;
     FastLED.show();
     delay(1);
     leds[whiteLed] = CRGB::Black;
  }      
}