#include <ArduinoJson.h>

class JSON
{
    private:
    public:
        char jsonTX[255];
        char jsonRX[255];
        StaticJsonDocument<255> JSONTX;
        StaticJsonDocument<255> JSONRX;
        
        void createStatus();
        void receiveMessage(String json);
};