#include <Arduino.h>
#include <PubSubClient.h>
#include <ETH.h>
// #include <Puzzle.h>

const char *mqtt_server = "192.168.1.208";
String clientId = "Client101";

WiFiClient espClient;
PubSubClient client(espClient);

void WiFiEvent(WiFiEvent_t event);
void testClient(const char *host, uint16_t port);
void callback(char *topic, byte *payload, unsigned int length);
void printIncomingMessage(char *_topic, byte *payload, unsigned int length);
void connect();
void server();

void HandleData(byte *payload);
void HandleAction(byte *payload);
void HandleGet(byte *payload);
void HandleSet(byte *payload);

static bool eth_connected = false;
long lastMsg = 0;
char msg[50];
int value = 0;

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(34, INPUT);

  Serial.begin(115200);
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  client.setServer(mqtt_server, 1234);
  client.setCallback(callback);
}

void loop()
{
    server();
}

void HandleData(byte *payload)
{
    Serial.println("Data requested...");
}

void HandleAction(byte *payload)
{
    Serial.println("Action requested...");
}

void HandleGet(byte *payload)
{
    Serial.println("Get requested...");
}

void HandleSet(byte *payload)
{
    Serial.println("Set requested...");
}

///////////////////// MQTT ///////////////////

void server()
{
    if (eth_connected)
    {
        if (!client.connected())
        {
          connect();
        }
        client.loop();

        int buttonPress = digitalRead(34);  
        if(buttonPress)
        {
            client.publish("Client101/Event", "ButtonPressed");
            delay(1000);
        }
    }
}

void callback(char *_topic, byte *payload, unsigned int length)
{
    printIncomingMessage(_topic, payload, length);

    String topic = _topic;

    digitalWrite(2, HIGH); // Turn the LED on (Note that LOW is the voltage level   

    if (topic == "Client101/Data")
    {
        HandleData(payload);
    }
    else if (topic == "Client101/Action")
    { 
        HandleAction(payload);
    }
    else if (topic == "Client101/Get")
    {
        HandleGet(payload);
    }
    else if (topic == "Client101/Set")
    {
        HandleSet(payload);
    }

    digitalWrite(2, LOW); // Turn the LED off by making the voltage HIGH
}

void printIncomingMessage(char *_topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(_topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();  
}

///////////////////// SETUP ///////////////////
void setupSubcriptions() //TODO get subrscribers out of array at top.
{
  String subscribers [4] = { "Data", "Action", "Get", "Set"};
  for (size_t i = 0; i < 4; i++)
  {
    String subName = clientId;
    subName += "/";
    subName += subscribers[i];
    Serial.print("Subscribing to [");
    Serial.print(subName);
    Serial.println("]");
    client.subscribe(subName.c_str());
  }  
}

void connect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Client101/Action/BlinkLed", "Connected");
      // ... and resubscribe
      setupSubcriptions();
    }
    else
    {
      Serial.print("failed to connect=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void WiFiEvent(WiFiEvent_t event)
{
  switch (event)
  {
  case SYSTEM_EVENT_ETH_START:
    Serial.println("ETH Started");
    //set eth hostname here
    ETH.setHostname("esp32-ethernet");
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

void testClient(const char *host, uint16_t port)
{
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port))
  {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available())
    ;
  while (client.available())
  {
    Serial.write(client.read());
  }

  Serial.println("closing connection\n");
  client.stop();
}
