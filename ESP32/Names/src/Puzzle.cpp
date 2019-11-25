#include <Arduino.h>
#include <Puzzle.h>

void Puzzle::Loop()
{
    // fill_rainbow( leds, NUM_LEDS, gHue, 7);
    // FastLED.delay(1000/60);
    // FastLED.show();   
    // EVERY_N_MILLISECONDS( 20 ) { gHue++; }
    for (int i = 0; i < ledAmount; i++)
    {
        leds[i] = CRGB::White;
        FastLED.show();
        delay(100);
        leds[i] = CRGB::Black;
        FastLED.show();
        Serial.println(i);
    }
}

void Puzzle::Reset()
{
    Serial.println("Resetting puzzle");
}

void Puzzle::LoadPuzzle(Preferences p)
{
    delay(1000);
    preferences = p;
    preferences.begin("puzzle", false);
    blinkTime = preferences.getUInt("blinkTime", 0);
    Serial.print("Loaded [blinkTime] : ");
    Serial.println(blinkTime);
    preferences.end();

    FastLED.addLeds<WS2811, 4, RGB>(leds, ledAmount);
    FastLED.setBrightness(128);
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

void Puzzle::BlinkLed()
{
    Serial.println("Turn led on..");
    Serial.print("Wait for ");
    Serial.println(blinkTime);
    Serial.println("Turn led off");
}

void Puzzle::SetBlinkTime(int bTime)
{
    blinkTime = bTime;
    preferences.begin("puzzle", false);
    preferences.putUInt("blinkTime", blinkTime);
    preferences.end();
    Serial.print("Set blinkTime to: ");
    Serial.println(blinkTime);
}

int Puzzle::GetBlinkTime()
{
    Serial.println(blinkTime);
    return blinkTime;
}