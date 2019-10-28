#include <Arduino.h>
#include <TCP.h>
#include <JSON.h>
#include <Request.h>

Preferences preferences;
TCP tcp;
JSON json;
Puzzle puzzle;
Request request(puzzle);  

long previousMillis = 0; 

int blinkTime = 1000;

void setup()
{
    Serial.begin(115200);
    delay(10);
    puzzle.injectPreferences(preferences);
    tcp.connectWifi();
    json.createStatus();  
}

void loop()
{   
    if(tcp.newMessage)
    {
        request.request = json.parseJson(tcp.messageRX);
        request.handleRequest();
        tcp.newMessage = false;
    }
    
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > tcp.interval) {
        previousMillis = currentMillis; 
        tcp.sendData(json.jsonTX);
    }

    Serial.println(request.puzzle.getBlinkTime());
}
