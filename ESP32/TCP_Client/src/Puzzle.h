#include <Preferences.h>

class Puzzle 
{
    public:
        Preferences preferences; 
        void BlinkLed();
        void setBlinkTime(int bTime);
        int getBlinkTime();
        Puzzle()
        {
            loadVariables();
        }
    private:
        int blinkTime; 
        void loadVariables();
};
