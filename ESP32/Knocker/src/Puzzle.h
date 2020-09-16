#include <Arduino.h>
#include <Preferences.h>
#include <MQTT.h>
#include <MCP23S17.h>

#define DOOR_1 1
#define DOOR_2 2

#define KNOCKER 3
#define LIGHT_REBUS 4

#define SIT_SENSOR_1 9
#define SIT_SENSOR_2 10

#define LIGHT_1 1
#define LIGHT_2 2
#define LIGHT_3 3

#define KNOCK_SENSOR 4

class Puzzle
{
    public:
        void LoadPuzzle(Preferences p);
        void LoadMQTT(MQTTClient *mqttClient, String _clientId);
        void Reset();
        void Loop();

        void LoadIO(MCP *_IO_1, MCP *_IO_2);

        void TurnOnLight(uint8_t lamp);
        void TurnOffLight(uint8_t lamp);
        void OpenDoor(uint8_t door);
        void CloseDoor(uint8_t door);
        void KnockHint(uint8_t knockAmount);

        void SetOpenValue(int value);
        void SetClosedValue(int value);

        int GetOpenValue();
        int GetClosedValue();

    private:
        Preferences preferences;
        MQTTClient *mqtt;
        String clientId;
        MCP *IO_RELAYBOARD;
        MCP *IO_2;

        void Knocker();
        bool CheckTarget();
};