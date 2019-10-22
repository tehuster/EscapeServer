#include <RequestHandler.h>


RequestHandler::requestEnum RequestHandler::getRequestType(const char* requestType)
{
    requestEnum rType = Status;
    String rTypeString = requestType;
    
    if(rTypeString == "data")
    {
        rType = Data;
    }else if(rTypeString == "action")
    {
        rType = Action;
    }else if(rTypeString == "config_get")
    {
        rType = Config_get;
    }
    else if(rTypeString == "config_set")
    {
        rType = Config_set;
    }

    return rType;
}

void RequestHandler::handleRequest()
{
    //const char* name = Request["name"];   
    const char* requestType = RequestHandler::request["requestType"];  
    
    switch (RequestHandler::getRequestType(requestType))
    {
    case Status:
        Serial.println("Send Status");
        break;
    case Data:
        Serial.println("Send data");
        break;
    case Action:
        Serial.println("Trigger Action");
        break;
    case Config_get:
        Serial.println("Send config");
        break;
    case Config_set:
        Serial.println("Set config");
        break;    
    default:
        Serial.println("Unknown requestType received");
        break;
    }
}