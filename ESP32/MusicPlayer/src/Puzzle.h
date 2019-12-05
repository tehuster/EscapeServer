#include <Preferences.h>
#include <DFRobotDFPlayerMini.h>
#include <MQTT.h>


class Puzzle
{
public:
    void LoadPuzzle(Preferences p);
    void LoadMQTT(MQTTClient *mqttClient, String _clientId);
    void Reset();
    void Loop();    

    void PlaySong(byte num);
    void SetVolume(int volume);   
    int CheckStatus();    

private:
    Preferences preferences;  
    MQTTClient *mqtt; 
    String clientId;
    DFRobotDFPlayerMini audio;
    void InitAudio();
    void printDetail(uint8_t type, int value);
    bool isPlaying;
    int volume;
};