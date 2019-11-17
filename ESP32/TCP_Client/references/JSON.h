#include <ArduinoJson.h>

class JSON
{
    private:
    public:
        char jsonTX[255];
        char jsonRX[255];
        StaticJsonDocument<255> Response;
        StaticJsonDocument<255> Request;
        
        void createStatus();
        StaticJsonDocument<255> parseJson(String json);
};