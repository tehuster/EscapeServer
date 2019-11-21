#include <Preferences.h>

class Puzzle 
{
    public:
        void LoadVariables(Preferences p);
        void BlinkLed();
        void SetBlinkTime(int bTime);        
        int GetBlinkTime();
    private:
        Preferences preferences; 
        int blinkTime;         
};