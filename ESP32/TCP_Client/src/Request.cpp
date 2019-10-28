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
    String actionName = Request::request["actionName"];
    String actionValue = Request::request["actionParameter"];

    if (actionName == "BlinkLed")
    {
        Request::puzzle.BlinkLed();
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
    String variableName = Request::request["actionName"];
    String variableValue = Request::request["actionParameter"];

    if (variableName == "blinkTime")
    {
        Request::puzzle.setBlinkTime(variableValue.toInt());
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
    String variableName = Request::request["actionName"];
    String variableValue = Request::request["actionParameter"];

    if (variableName == "blinkTime")
    {
        Serial.print("blinkTime: ");
        Serial.println(Request::puzzle.getBlinkTime());
    }
    else
    {
        Serial.print("Unknown: VariableName: ");
        Serial.println(variableName);
    }
}

void Request::handleRequest()
{
    String rTypeString = Request::request["requestType"];

    if (rTypeString == "status")
    {
        Request::Status();
    }else if (rTypeString == "data")
    {
        Request::Data();
    }
    else if (rTypeString == "action")
    {
        Request::Action();
    }
    else if (rTypeString == "config_get")
    {
        Request::Config_get();
    }
    else if (rTypeString == "config_set")
    {
        Request::Config_set();
    }
    else
    {
        Serial.println("Unknown: RequestType");
    }
}