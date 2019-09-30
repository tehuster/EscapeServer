#include "JSON.h"

void JSON::createStatus()
{
    StaticJsonDocument<255> JSON;

    JsonObject status = JSON.createNestedObject("status");

    status["name"] = "Client101";
    status["address"] = 101;
    status["status"] = 69;
    status["rssi"] = -36;

    serializeJson(JSON, jsonBuffer);

    //Serial.println();
    //serializeJsonPretty(JSON, Serial);
}