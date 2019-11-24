#include <Preferences.h>
#include <FastLED.h>

//6,  0  - 5
//8,  6  - 13
//13, 14 - 26
//14, 27 - 40
//18, 41 - 58
#define DATA_PIN    4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    59

class Puzzle 
{
    public:
        void LoadPuzzle(Preferences p);
        void Reset();
        void Loop();

        void TurnOffLeds();
        
        void BlinkLed();
        void SetBlinkTime(int bTime);        
        int GetBlinkTime();
    private:
        CRGB leds[NUM_LEDS];
        uint8_t gHue = 0;
        const int ledAmount = NUM_LEDS;
        const int nameLedAmount[5] = {6, 8, 13, 14, 18}; 
        const int nameLedBegin[5]  = {0, 6, 14, 27, 41}; //Do we need this? It's current nameLedAmount + the previous ones?
        Preferences preferences; 
        int blinkTime;         
};