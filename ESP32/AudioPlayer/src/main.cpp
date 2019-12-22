#include <Arduino.h>
#include <ETH.h>
#include <MQTT.h>
#include <Puzzle.h>

WiFiClient ethernetConnection;
MQTTClient mqtt;
Preferences preferences;
Puzzle puzzle;

const char *mqtt_server = "192.168.2.3";
String clientId = "AudioPlayer";
String subscribers[4] = {"Data", "Action", "Get", "Set"};

const int IO_1_SS = 13;
MCP IO_1(0, IO_1_SS); 

void HandleData(String payload);
void HandleAction(String payload);
void HandleGet(String payload);
void HandleSet(String payload);
String GetValue(String data, char separator, int index);

void MQTT_Update();
void MessageReceived(String &topic, String &payload);
void Connect();
void InitMQTT();
void WiFiEvent(WiFiEvent_t event);
static bool eth_connected = false;

unsigned long runTime = 0;

void setup()
{
  Serial.begin(115200);
  
  pinMode(IO_1_SS, OUTPUT);
  digitalWrite(IO_1_SS, HIGH);

  IO_1.begin();

  for (int i = 1; i < 9; i++) {    
    IO_1.pinMode(i, LOW);      // Use bit-write mode to Set all of the current pin on outputchip to be an output
  }
  for (int i = 9; i < 16; i++) {    // Since we are only workign with one bit at a time, use a loop to take action each pin (0-15)
    IO_1.pinMode(i, HIGH);      // Use bit-write mode to set all of the current pin on inputchip to be inputs
    IO_1.pullupMode(i, HIGH);   // Use bit-write mode to Turn on the internal pull-up resistor for the current pin
    IO_1.inputInvert(i, LOW);  // Use bit-write mode to invert the input so that logic 0 is read as HIGH
  }
  
  pinMode(2, OUTPUT);
  pinMode(34, INPUT);  

  InitMQTT(); 
  puzzle.LoadIO(&IO_1);
  puzzle.LoadPuzzle(preferences);  
  puzzle.LoadMQTT(&mqtt, clientId);  
}

void loop()
{ 
    runTime = millis();
    MQTT_Update();
    puzzle.Loop();    
}

///////////////////// TOPICS ///////////////////

void HandleData(String payload)
{
  String runTimeMsg = String(runTime);
  Serial.println("Data requested...");
  mqtt.publish(clientId + "/Response/Data", runTimeMsg, true, 1);
}

void HandleAction(String payload)    
{
  Serial.println("Action requested...");
  String name = GetValue(payload, '/', 0);
  String value = GetValue(payload, '/', 1);
  String response;

  if(name == "Reset")
  {
    puzzle.Reset();
    response = "Reset";
  }
  else if(name == "OpenDrawer")
  {
    puzzle.OpenDrawer();
    String status = "Opening Drawer";
    response = status;
    Serial.println("Opening Drawer");
  }
  else if(name == "CloseDrawer")
  {
    puzzle.CloseDrawer();
    String status = "Closing Drawer";
    response = status;    
    Serial.println("Closing Drawer");
  }
  else if(name == "PlayAudio")
  {
    puzzle.PlayAudio(value.toInt());
    String status = puzzle.CheckStatus();
    response = status;
  }
  else if(name == "SetVolume")
  {
    puzzle.SetVolume(value.toInt());
    response = "SetVolume: " + value;
  }
  else if(name == "CheckStatus")
  {
    String status = puzzle.CheckStatus();
    response = status;
  }
  else
  {
    Serial.print("Requested unknown action: ");
    Serial.println(name);
    response = "Unknown Action";    
  }
  mqtt.publish(clientId + "/Response/Action", response, true, 1);
}

void HandleGet(String payload)
{
  Serial.println("Get requested...");
  String name = GetValue(payload, '/', 0);
  String value = GetValue(payload, '/', 1);
  if (name == "blinkTime")
  {
    // int blinkTime = puzzle.GetBlinkTime();
    // mqtt.publish(clientId + "/Response/Get", String(blinkTime), true, 1);
  }
  else
  {
    Serial.print("Requested unknown get: ");
    Serial.println(name);
    mqtt.publish(clientId + "/Response/Get", "Unknown Get", true, 1);
  }
}

void HandleSet(String payload)
{
  Serial.println("Set requested...");
  String name = GetValue(payload, '/', 0);
  String value = GetValue(payload, '/', 1);
  if (name == "blinkTime")
  {
    // puzzle.SetBlinkTime(value.toInt());
    // mqtt.publish(clientId + "/Response/Set", value, true, 1);
  }
  else
  {
    Serial.print("Requested unknown set: ");
    Serial.println(name);
    mqtt.publish(clientId + "/Response/Set", "Unknown Set", true, 1);
  }
}

///////////////////// UTILITY ///////////////////

String GetValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

///////////////////// MQTT ///////////////////

void MQTT_Update()
{
  if (eth_connected)
  {
    if (!mqtt.connected())
    {
      Connect();
    }
    else
    {
      mqtt.loop();
    }
  }
  int buttonPress = digitalRead(34);
  if (buttonPress)
  {
    mqtt.publish(clientId + "/Event", "Button is pressed!", true, 1);
    delay(1000);
  }
}

void MessageReceived(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);

  digitalWrite(2, HIGH); // Turn the LED on (Note that LOW is the voltage level

  if (topic == clientId + "/Data")
  {
    HandleData(payload);
  }
  else if (topic == clientId + "/Action")
  {
    HandleAction(payload);
  }
  else if (topic == clientId + "/Get")
  {
    HandleGet(payload);
  }
  else if (topic == clientId + "/Set")
  {
    HandleSet(payload);
  }

  digitalWrite(2, LOW); // Turn the LED off by making the voltage HIGH
}

void Connect()
{
  while (!mqtt.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(clientId.c_str()))
    {
      Serial.println("Connected with MQTT broker");
      mqtt.publish(clientId + "/Event", "Connected", true, 1);

      for (size_t i = 0; i < 4; i++)
      {
        String subName = clientId;
        subName += "/";
        subName += subscribers[i];
        Serial.print("Subscribing to [");
        Serial.print(subName);
        Serial.println("]");
        mqtt.subscribe(subName.c_str());
      }
    }
    else
    {
      Serial.print("Failed to connect to broker");
      Serial.println(" trying again in 2 seconds");
      delay(2000);
    }
  }
}

void InitMQTT()
{
  WiFi.onEvent(WiFiEvent);
  ETH.begin();

  mqtt.begin(mqtt_server, ethernetConnection);
  String willTopic = clientId + "/Error";
  mqtt.setWill(willTopic.c_str(), "Device disconnected", true, 2);  //<-----------------
  mqtt.setOptions(5, true, 777);
  mqtt.onMessage(MessageReceived);
}

void WiFiEvent(WiFiEvent_t event)
{
  switch (event)
  {
  case SYSTEM_EVENT_ETH_START:
    Serial.println("ETH Started");
    ETH.setHostname(clientId.c_str());
    break;
  case SYSTEM_EVENT_ETH_CONNECTED:
    Serial.println("ETH Connected");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP:
    Serial.print("ETH MAC: ");
    Serial.print(ETH.macAddress());
    Serial.print(", IPv4: ");
    Serial.print(ETH.localIP());
    if (ETH.fullDuplex())
    {
      Serial.print(", FULL_DUPLEX");
    }
    Serial.print(", ");
    Serial.print(ETH.linkSpeed());
    Serial.println("Mbps");
    eth_connected = true;
    break;
  case SYSTEM_EVENT_ETH_DISCONNECTED:
    Serial.println("ETH Disconnected");
    eth_connected = false;
    break;
  case SYSTEM_EVENT_ETH_STOP:
    Serial.println("ETH Stopped");
    eth_connected = false;
    break;
  default:
    break;
  }
}