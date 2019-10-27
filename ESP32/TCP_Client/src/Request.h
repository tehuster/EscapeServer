#include <Arduino.h>
#include <ArduinoJson.h>
#include <Puzzle.h>

class Request
{
    public:    
        Puzzle puzzle;
        Request(Puzzle _puzzle)
        {
            puzzle = _puzzle;
        }
        StaticJsonDocument<255> request;
        void handleRequest();
    private:
        void Status();
        void Data();
        void Action();
        void Config_set();
        void Config_get();
};
