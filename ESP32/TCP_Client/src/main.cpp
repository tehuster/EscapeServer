#include <Arduino.h>
#include <TCP.h>
#include <JSON.h>
#include <RequestHandler.h>

TCP tcp;
JSON json;
RequestHandler requestHandler;  

long previousMillis = 0; 

void setup()
{
    Serial.begin(115200);
    delay(10);

    tcp.connectWifi();
    json.createStatus();   
}

void loop()
{   
    if(tcp.newMessage)
    {
        requestHandler.request = json.parseJson(tcp.messageRX);
        requestHandler.handleRequest();
        tcp.newMessage = false;
    }
    
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > tcp.interval) {
        previousMillis = currentMillis; 
        tcp.sendData(json.jsonTX);
    }
}

