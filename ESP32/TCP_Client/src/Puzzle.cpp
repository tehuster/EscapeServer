#include <Arduino.h>
#include <Puzzle.h>

void Puzzle::blinkLed()
{
    Serial.println("Turn led on..");
    Serial.print("Wait for ");
    Serial.print(blinkTime);
    Serial.println("Turn led off");    
}
