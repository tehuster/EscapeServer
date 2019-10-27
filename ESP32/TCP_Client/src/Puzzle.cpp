#include <Arduino.h>
#include <Puzzle.h>

void Puzzle::BlinkLed()
{
    Serial.println("Turn led on..");
    Serial.print("Wait for ");
    Serial.println(blinkTime);
    Serial.println("Turn led off");    
}
