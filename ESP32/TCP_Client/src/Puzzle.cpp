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
    blinkTime = bTime;    
    preferences.begin("puzzle", false);
    preferences.putUInt("blinkTime", blinkTime);
    preferences.end();
}

int Puzzle::getBlinkTime()
{
    Serial.println(blinkTime);
    return blinkTime;
}

void Puzzle::loadPreferences(Preferences& preferences)  //"&" pass by reference (memory address), not by value (copy of memory).
{
    Serial.print("Getting preferences variables: ");
    preferences.begin("puzzle", false);
    blinkTime = preferences.getUInt("blinkTime", 0);
    preferences.end();
    Serial.println(blinkTime);
}

        
