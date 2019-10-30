#include <TCP.h>

void TCP::Event(WiFiEvent_t event)
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
        //TCP::eth_connected = true;
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        Serial.println("ETH Disconnected");
        //TCP::eth_connected = false;
        break;
    case SYSTEM_EVENT_ETH_STOP:
        Serial.println("ETH Stopped");
        //TCP::eth_connected = false;
        break;
    default:
        break;
    }
}

void TCP::sendData(const char *data)
{
    WiFiClient client;
    if (!client.connect(host, port))
    {
        Serial.println("connection failed");
        return;
    }
    client.print(data);

    while (client.connected() && !client.available());
    while (client.available())
    {
        messageRX = client.readStringUntil('\r');
    }
    newMessage = true;
    //Serial.println("closing connection\n");
    client.stop();
}


// void TCP::connectWifi()
// {
//     // We start by connecting to a WiFi network
//     WiFi.begin("Huster 2.4", "N11ico11e");    
//     Serial.print("Waiting for WiFi... ");

//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }

//     Serial.println("");
//     Serial.println("WiFi connected");
//     Serial.println("IP address: ");
//     Serial.println(WiFi.localIP());
//     delay(500);
// }

// void TCP::sendData(const char* data)
// {
//     // Use WiFiClient class to create TCP connections
//     WiFiClient client;

//     if (!client.connect(host, port)) {
//         Serial.println("Connection failed.");
//         Serial.println("Waiting 5 seconds before retrying...");
//         delay(1000);
//         return;
//     }

//     // This will send a request to the server
//     client.print(data);

//     unsigned long timeout = millis();

//     while (client.available() == 0) {
//         if (millis() - timeout > 1000) {
//             Serial.println(">>> Client Timeout !");
//             client.stop();
//             return;
//         }
//     }
//     while(client.available()) {
//         messageRX = client.readStringUntil('\r');
//     } 
//     newMessage = true;
//     client.stop();
// }