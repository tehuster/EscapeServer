#include <Arduino.h>
#include <Preferences.h>
#include <MQTT.h>
#include <Adafruit_PN532.h>
#include <esp32-hal-ledc.h>

#define PN532_SCK  (14)
#define PN532_MOSI (16)
#define PN532_MISO (15)
#define PN532_SS   (32)

#define COUNT_LOW 3200    //1ms
#define COUNT_HIGH 6400  //2ms
#define TIMER_WIDTH 16

class Puzzle
{
public:
    void LoadPuzzle(Preferences p);
    void LoadMQTT(MQTTClient *mqttClient, String _clientId);
    void Reset();
    void Loop();  

    void Open();
    void Close();
    void Toggle();    

    void SetOpenValue(int value); 
    void SetClosedValue(int value); 

    int GetOpenValue(); 
    int GetClosedValue(); 

private:
    Adafruit_PN532 *rfid;
    Preferences preferences;  
    MQTTClient *mqtt; 
    String clientId;   
    String old_rfidTag;

    int open = 3200;
    int closed = 6000;

    uint8_t success;
    uint8_t uid[7] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID    
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint8_t packetbuffer[3];
};