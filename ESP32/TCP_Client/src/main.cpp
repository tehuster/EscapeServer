#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

const uint16_t port = 8080;
const char * host = "192.168.1.249"; // ip or dns
char jsonBuffer[255];

void sendData(const char* d);

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFi.begin("Huster 2.4", "N11ico11e");

    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);

    StaticJsonDocument<255> JSON;

    JsonObject status = JSON.createNestedObject("status");

    status["name"] = "Client101";
    status["address"] = 101;
    status["status"] = 69;
    status["rssi"] = -36;

    serializeJson(JSON, jsonBuffer);
    //Serial.println();
    //serializeJsonPretty(JSON, Serial);
}


void loop()
{
    sendData(jsonBuffer);
    delay(10000);
}

void sendData(const char* data)
{
    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        delay(5000);
        return;
    }

    // This will send a request to the server
    client.print(data);

    unsigned long timeout = millis();

    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }
    //read back one line from the server, NEED TO WAIT FOR THIS.
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.println(line);
    }    

    //Serial.println("Closing connection.");    
    client.stop();
}