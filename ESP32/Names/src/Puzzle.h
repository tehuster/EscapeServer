#include <Preferences.h>
#include <SPI.h>

//6,  0  - 5
//8,  6  - 13
//13, 14 - 26
//14, 27 - 40
//18, 41 - 58

#define SETLED 0
#define SHOWLEDS 1

#define NUM_LEDS 10
#define BRIGHTNESS 200  //TODO: Brightness action for slave

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
    
    const int ledAmount = NUM_LEDS;
    const int nameLedAmount[5] = {6, 8, 13, 14, 18};
    const int nameLedBegin[5] = {0, 6, 14, 27, 41}; //Do we need this? It's current nameLedAmount + the previous ones?

    int writeSpeed = 10;
    void WriteName(int start, int length, int speed);
    void FlowName(int start, int length, int speed);
    void StampName(int start, int length);

    void SetLed(int ledIndex, int r, int g, int b);
    void ShowLeds();
};