#include <Arduino.h>
#include <ArduinoJson.h>

class RequestHandler
{
    public:    
        enum requestEnum
        {
            Status,
            Data,
            Action,
            Config_get,
            Config_set
        };
        StaticJsonDocument<255> request;
        requestEnum getRequestType(const char *requestType);
        void handleRequest();
    private:
};
