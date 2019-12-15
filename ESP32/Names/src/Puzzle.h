#include <Preferences.h>
#include <SPI.h>

//54,  0  - 54
//42,  55 - 97
//47, 98 - 145
//42, 146 - 188
//39, 189 - 228
//45    229 - 274



#define SETLED 0
#define SHOWLEDS 1

#define NUM_LEDS 274
#define BRIGHTNESS 200  //TODO: Brightness action for slave

enum Name
{
    Tiny = 0,
    Small = 1,
    Medium = 2,
    Big = 3,
    Huge = 4,
    Enourmous = 5
};

class Puzzle
{
public:
    void LoadPuzzle(Preferences p);
    void Reset();
    void Loop();

    void TurnOffLeds();
    void ShowName(int name);
    void Hint();

private:
    Preferences preferences;

    //54,  0  - 54
    //42,  55 - 97
    //47, 98 - 145
    //42, 146 - 188
    //39, 189 - 228
    //45    229 - 274
    
    const int ledAmount = NUM_LEDS;
    const int nameLedAmount[6] = {54, 42, 47, 42, 39, 45};
    const int nameLedBegin[6] = {0, 55, 98, 146, 189, 229}; //Do we need this? It's current nameLedAmount + the previous ones?

    int writeSpeed = 10;
    void WriteName(int start, int length, int speed);
    void FlowName(int start, int length, int speed);
    void StampName(int start, int length);

    void SetLed(int ledIndex, int r, int g, int b);
    void ShowLeds();
};