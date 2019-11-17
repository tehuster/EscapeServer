#include <Arduino.h>
#include <Preferences.h>
#include <Coms.h>

bool connected = false;

Preferences preferences;
Coms coms;

long previousMillis = 0; 

int blinkTime = 1000;

void setup()
{
    Serial.begin(115200);
    WiFi.onEvent(coms.Event);
    ETH.begin();
    delay(10);
    coms.loadPreferences(preferences);    
    coms.createStatus();  
}

void loop()
{   
    if(coms.newMessage)
    {        
        coms.handleRequest();
        coms.newMessage = false;
    }
    
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > coms.interval) {
        previousMillis = currentMillis; 
        coms.sendData(coms.jsonResponse);
    }
}
