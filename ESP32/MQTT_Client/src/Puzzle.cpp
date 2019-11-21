#include <Arduino.h>
#include <Puzzle.h>

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

void Puzzle::LoadVariables(Preferences p)
{
    preferences = p;
    preferences.begin("puzzle", false);
    blinkTime = preferences.getUInt("blinkTime", 0);
    Serial.print("Loaded [blinkTime] : ");
    Serial.println(blinkTime);
    preferences.end();
}

int Puzzle::GetBlinkTime()
{
    Serial.println(blinkTime);
    return blinkTime;
}