#include <Request.h>

void Request::Status()
{
    //Serial.println("RequestType: Status");
}

void Request::Data()
{
    Serial.println("RequestType: Data");
}

void Request::Action()
{
    Serial.println("RequestType: Action");
    String actionName = request["actionName"];
    String actionValue = request["actionParameter"];

    if (actionName == "BlinkLed")
    {
        puzzle.BlinkLed();
    }
    else
    {
        Serial.print("Unknown: ActionName: ");
        Serial.println(actionName);
    }
}

void Request::Config_set()
{
    Serial.println("RequestType: Config_set");
    String variableName = request["actionName"];
    String variableValue = request["actionParameter"];

    if (variableName == "blinkTime")
    {
        puzzle.setBlinkTime(variableValue.toInt());
        Serial.print("blinkTime: ");
        Serial.println(puzzle.getBlinkTime());        
    }
    else
    {
        Serial.print("Unknown: VariableName: ");
        Serial.println(variableName);
    }
}

void Request::Config_get()
{   
    Serial.println("RequestType: Config_get");
    String variableName = request["actionName"];
    String variableValue = request["actionParameter"];

    if (variableName == "blinkTime")
    {
        Serial.print("blinkTime: ");
        Serial.println(puzzle.getBlinkTime());
    }
    else
    {
        Serial.print("Unknown: VariableName: ");
        Serial.println(variableName);
    }
}

void Request::handleRequest()
{
    String rTypeString = request["requestType"];
    //#include <map>
    //std::map<T, T> key value pair dictionairy voor generieke toekomst.

    //#include <unordered_map>
    //typedef void (*RequestFunc)(void); // function pointer type
    //typedef std::unordered_map<String, RequestFunc> funcMap;
    //funcMap requestFuncs;
    //requestFuncs.emplace("appel", &Status);
    
    if (rTypeString == "status")
    {
        Status();
    }else if (rTypeString == "data")
    {
        Data();
    }
    else if (rTypeString == "action")
    {
        Action();
    }
    else if (rTypeString == "config_get")
    {
        Config_get();
    }
    else if (rTypeString == "config_set")
    {
        Config_set();
    }
    else
    {
        Serial.println("Unknown: RequestType");
    }
}

void Request::loadPreferences(Preferences &preferences)  //"&" pass by reference (memory address), not by value (copy of memory).
{
    Serial.print("Getting preferences variables: ");
    preferences.begin("puzzle", false);
    puzzle.setBlinkTime(preferences.getUInt("blinkTime", 0));
    preferences.end();
    Serial.println(puzzle.getBlinkTime());
}
