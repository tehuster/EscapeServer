#include <Preferences.h>

class Puzzle 
{
    public:
        Preferences preferences; 
        void BlinkLed();
        void setBlinkTime(int bTime);
        int getBlinkTime();
    private:
        int blinkTime; 
        void loadVariables();
};
