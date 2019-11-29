#include <Arduino.h>
#include <Puzzle.h>


unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 25;           // interval at which to blink (milliseconds)
int counter = 0;
void Puzzle::Loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        // set the LED with the ledState of the variable:
        leds[counter] = CRGB::White;
        FastLED.show();
        leds[counter] = CRGB::Black;
        FastLED.show();

         counter ++;
        if(counter >= NUM_LEDS)
        {
            counter = 0;
        }
    }
    
   
    // ShowName(Tiny, false);
    // delay(2500);
    // TurnOffLeds();
    // ShowName(Small, true);
    // delay(2500);
    // TurnOffLeds();
    // ShowName(Medium, true);
    // delay(2500);
    // TurnOffLeds();
    // ShowName(Big, true);
    // delay(2500);
    // TurnOffLeds();
    // ShowName(Huge, true);
    // delay(2500);    
    // TurnOffLeds();
}

void Puzzle::Reset()
{
    Serial.println("Resetting puzzle");
}

void Puzzle::LoadPuzzle(Preferences p)
{
    delay(1000);
    preferences = p;
    // preferences.begin("puzzle", false);
    // blinkTime = preferences.getUInt("blinkTime", 0);
    // Serial.print("Loaded [blinkTime] : ");
    // Serial.println(blinkTime);
    preferences.end();

    FastLED.addLeds<WS2811, 4, RGB>(leds, ledAmount);
    FastLED.setBrightness(BRIGHTNESS);
    TurnOffLeds();
}

void Puzzle::TurnOffLeds()
{
    for (int i = 0; i < ledAmount; i++)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void Puzzle::ShowName(Name name, bool writeName)
{
    if(writeName)
    {
        FlowName(nameLedBegin[name], nameLedAmount[name], writeSpeed);
    }else
    {
        StampName(nameLedBegin[name], nameLedAmount[name]);
    }
    
}

void Puzzle::Hint()
{

}

void Puzzle::WriteName(int start, int length, int speed)
{
    for (int i = start; i < start + length; i++)
    {
        leds[i] = color;
        FastLED.delay(speed);
        FastLED.show();
    }   
}

void Puzzle::FlowName(int start, int length, int speed)
{
    for (int i = start; i < start + length; i++)
    {
        for (int b = 0; b < 200; b++)
        {
            leds[i].green = b;
            FastLED.delay(speed);   
            FastLED.show();
        }   
    }   
}

void Puzzle::StampName(int start, int length)
{
    for (int i = start; i < start + length; i++)
    {
        leds[i] = color;        
    } 
    FastLED.show();
}   