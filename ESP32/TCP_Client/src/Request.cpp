#include <Request.h>

void Request::Status()
{
    Serial.println("RequestType: Status");
}

void Request::Data()
{
    Serial.println("RequestType: Data");
}

void Request::Action()
{
    Serial.println("RequestType: Action");
    String actionName = Request::request["actionName"];
    String actionValue = Request::request["actionValue"];

    if (actionName == "blinkLed")
    {
        Request::puzzle.blinkLed();
    }
    else
    {
        Serial.println("Unknown: ActionName");
    }
}

void Request::Config_set()
{
    Serial.println("RequestType: Config_set");
    String variableName = Request::request["variableName"];
    String variableValue = Request::request["variableValue"];

    if (variableName == "blinkLed")
    {
        Request::puzzle.blinkTime = variableValue.toInt();
    }
    else
    {
        Serial.println("Unknown: VariableName");
    }
}

void Request::Config_get()
{
    Serial.println("RequestType: Config_get");
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