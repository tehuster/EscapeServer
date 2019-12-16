#include <Arduino.h>
#include <Puzzle.h>

void Puzzle::Loop()
{
  
}

void Puzzle::Reset()
{
  Serial.println("Resetting puzzle");
}

void Puzzle::LoadPuzzle(Preferences p)
{
  delay(1000);
  Serial2.begin(9600, SERIAL_8N1, 32, 33);
  delay(1000);
  preferences = p;
  // preferences.begin("puzzle", false);
  // volume = preferences.getUInt("volume", 0);
  // Serial.print("Loaded [volume] : ");
  // Serial.println(volume);
  // preferences.end();    

  //TURN OF CHIPSELECT FOR IO EXPANDER
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    pinMode(32, OUTPUT);
    digitalWrite(32, HIGH); 

    pinMode(34, INPUT);
}

void Puzzle::LoadMQTT(MQTTClient *mqttClient, String _clientId)
{
  clientId = _clientId;
  mqtt = mqttClient;
}