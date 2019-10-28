#include <Arduino.h>
#include <Puzzle.h>

void Puzzle::BlinkLed()
{
    Serial.println("Turn led on..");
    Serial.print("Wait for ");
    Serial.println(blinkTime);
    Serial.println("Turn led off");    
}

void Puzzle::setBlinkTime(int bTime)
{
    Puzzle::blinkTime = bTime;    
    preferences.begin("puzzle", false);
    preferences.putUInt("blinkTime", Puzzle::blinkTime);
    preferences.end();
}

int Puzzle::getBlinkTime()
{
    Serial.println(Puzzle::blinkTime);
    return Puzzle::blinkTime;
}

void Puzzle::loadVariables()
{
    Serial.print("Getting preferences variables: ");
    preferences.begin("puzzle", false);
    Puzzle::blinkTime = preferences.getUInt("blinkTime", 0);
    preferences.end();
    Serial.println(Puzzle::blinkTime);
}

void Puzzle::injectPreferences(Preferences _preferences)
{
    Puzzle::preferences = _preferences;
    loadVariables();
}

        
