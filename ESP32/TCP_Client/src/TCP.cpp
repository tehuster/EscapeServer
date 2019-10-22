#include <TCP.h>

void TCP::connectWifi()
{
    // We start by connecting to a WiFi network
    WiFi.begin("Huster 2.4", "N11ico11e");    
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
}

void TCP::sendData(const char* data)
{
    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        delay(1000);
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
    while(client.available()) {
        messageRX = client.readStringUntil('\r');
    } 
    newMessage = true;
    client.stop();
}