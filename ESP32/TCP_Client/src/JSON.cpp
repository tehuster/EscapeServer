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
    DeserializationError err = deserializeJson(JSONRX, json);    
    
    if (err)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(err.c_str());  
    }

    const char* id = JSONRX["requestType"];     
    const char* address = JSONRX["address"];  
    const char* requestType = JSONRX["requestType"];  
    Serial.println(requestType);
}