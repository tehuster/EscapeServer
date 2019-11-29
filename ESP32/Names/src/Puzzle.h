#include <Preferences.h>
// #define FASTLED_RMT_BUILTIN_DRIVER false
// #define FASTLED_ESP32_I2S true
#include <FastLED.h>

//6,  0  - 5
//8,  6  - 13
//13, 14 - 26
//14, 27 - 40
//18, 41 - 58

#define DATA_PIN 4
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS 59
#define BRIGHTNESS 200

enum Name
{
    Tiny = 0,
    Small = 1,
    Medium = 2,
    Big = 3,
    Huge = 4
};

class Puzzle
{
public:
    void LoadPuzzle(Preferences p);
    void Reset();
    void Loop();

    void TurnOffLeds();
    void ShowName(Name name, bool writeName);
    void Hint();

private:
    Preferences preferences;

    CRGB leds[NUM_LEDS];
    const int ledAmount = NUM_LEDS;
    const int nameLedAmount[5] = {6, 8, 13, 14, 18};
    const int nameLedBegin[5] = {0, 6, 14, 27, 41}; //Do we need this? It's current nameLedAmount + the previous ones?

    int writeSpeed = 10;
    CRGB color = CRGB::Green;
    void WriteName(int start, int length, int speed);
    void FlowName(int start, int length, int speed);
    void StampName(int start, int length);
};