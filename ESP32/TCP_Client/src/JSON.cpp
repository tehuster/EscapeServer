#include "JSON.h"

void JSON::createStatus()
{
    JsonObject status = JSONTX.createNestedObject("status");

    status["name"] = "Client101";
    status["address"] = 10;
    status["status"] = 69;
    status["rssi"] = -36;

    serializeJson(JSONTX, jsonTX);
}

void JSON::receiveMessage(String json)
{   
    Serial.println(json);
    DeserializationError err = deserializeJson(JSONRX, json);    
    
    if (err)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(err.c_str());  
    }

    const char* id = JSONRX[0]["requestType"];     
    const char* address = JSONRX[0]["address"];  
    const char* requestType = JSONRX[0]["requestType"];     
}