#include <ETH.h>
#include <ArduinoJson.h>
#include <Preferences.h>

class Puzzle;

class Coms
{
    private:
        const uint16_t port = 8080;
        const char * host = "192.168.1.208"; 

        void Status();
        void Data();
        void Action();
        void Config_set();
        void Config_get();

    public:
        // Puzzle puzzle;

        StaticJsonDocument<255> request;  
        void loadPreferences(Preferences &_preferences);
        void handleRequest();

        long interval = 100;
        String messageRX;
        bool newMessage = false;
        static bool eth_connected;
        static void Event(WiFiEvent_t event);        
        void sendData(const char* d);

        char jsonResponse[255];
        char jsonRequest[255];
        StaticJsonDocument<255> Response;
        StaticJsonDocument<255> Request;        
        void createStatus();
        StaticJsonDocument<255> parseJson(String json);
};
