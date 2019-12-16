#include <Arduino.h>
#include <Preferences.h>
#include <MQTT.h>
#include <Adafruit_PN532.h>

#define PN532_SCK  14
#define PN532_MOSI 15
#define PN532_MISO 16
#define PN532_SS   32

class Puzzle
{
public:
    void LoadPuzzle(Preferences p);
    void LoadMQTT(MQTTClient *mqttClient, String _clientId);
    void Reset();
    void Loop();       

private:
    // Adafruit_PN532 nfc;
    Preferences preferences;  
    MQTTClient *mqtt; 
    String clientId;   
};