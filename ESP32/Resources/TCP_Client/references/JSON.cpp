#include <JSON.h>

void JSON::createStatus()
{
    JsonObject status = Response.createNestedObject("status");

    status["name"] = "Client101";
    status["requestType"] = 'data';

    serializeJson(Response, jsonTX);
}

StaticJsonDocument<255> JSON::parseJson(String json)
{   
    DeserializationError err = deserializeJson(Request, json);    
    
    if (err)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(err.c_str());  
    }    

    return Request;
}