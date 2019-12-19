#include <Preferences.h>
#include <SPI.h>

//54,  0  - 54
//42,  55 - 97
//47, 98 - 145
//42, 146 - 188
//39, 189 - 228
//45  229 - 274

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
#define BRIGHTNESSVALUE 200  //TODO: Brightness action for slave

class Puzzle
{
public:
    void LoadPuzzle(Preferences p);
    void Reset();
    void Loop();
    
    void TurnOffLeds();
    void ShowName(uint8_t name);
    void Hint(uint8_t type);
    void SetBrightness(uint8_t bright);
    void SetWriteSpeed(uint8_t speed);
    uint8_t GetBrightness();
    uint8_t GetWriteSpeed();
    void SetColor_R(uint8_t value);
    void SetColor_G(uint8_t value);
    void SetColor_B(uint8_t value);

private:
    Preferences preferences;

    uint8_t brightness;
    uint8_t writeSpeed;

    uint8_t bufferIndex = 0;
    uint8_t rfidBuffer[64];
    const uint8_t msgSize = 3;
    void WriteBuffer(uint8_t cmd);
    void SendBuffer();
    void PrintBuffer();
};