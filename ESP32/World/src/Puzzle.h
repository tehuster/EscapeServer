#include <Arduino.h>
#include <Preferences.h>
#include <MQTT.h>

#define RXD2 33
#define TXD2 32
#define DE 14
#define bautrate 115200

class Puzzle
{
public:
    void LoadPuzzle(Preferences p);
    void LoadMQTT(MQTTClient *mqttClient, String _clientId);
    void Reset();
    void Loop();  

   

private:
    Preferences preferences;  
    MQTTClient *mqtt; 
    String clientId;    
    uint8_t deviceCounter = 101; //101 <= 114
    uint8_t incomingByte = 0; 
    uint8_t incomming_byte_counter = 0;
    uint8_t ReceiveArray[7];   

    uint8_t SensorTresshold = 5; 
    uint8_t NodeSensorInitialValue[14] = {0,0,0,0,0,0,0, 0,0,0,0,0,0,0}; 
    uint8_t NodeSensorValue[14];    
    uint8_t NodeLedValues[14][3]; 
    unsigned long previousMillis = 0; 
    const long interval = 18;     

    void SendMessage(uint8_t adress, uint8_t R, uint8_t G, uint8_t B, uint8_t option);   
    void ReceiveMessage();
};