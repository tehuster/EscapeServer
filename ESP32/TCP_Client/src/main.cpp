#include <Arduino.h>
#include <TCP.h>
#include <JSON.h>

TCP tcp;
JSON json;  


void setup()
{
    Serial.begin(115200);
    delay(10);

    tcp.connectWifi();
    json.createStatus();      
}

void loop()
{
    tcp.sendData(json.jsonBuffer);
    delay(3000);
}

