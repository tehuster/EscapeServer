#include <Preferences.h>
#include <DFRobotDFPlayerMini.h>
#include <MQTT.h>
#include <MCP23S17.h>


class Puzzle
{
public:
    void LoadPuzzle(Preferences p);
    void LoadMQTT(MQTTClient *mqttClient, String _clientId);
    void Reset();
    void Loop();    

    void PlayAudio(byte num);
    void SetVolume(int volume);  
    void LoadIO(MCP *_IO_1); 

    void OpenDrawer();
    void CloseDrawer();
    
    String CheckStatus();    

private:
    Preferences preferences;  
    MQTTClient *mqtt; 
    String clientId;
    DFRobotDFPlayerMini audio;
    MCP *IO_1;    
    
    void InitAudio();
    void printDetail(uint8_t type, int value);
    bool isPlaying;
    int volume;
};