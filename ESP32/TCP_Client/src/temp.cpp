// //main.cpp
// #include <Coms.h>

// Puzzle puzzle;
// Puzzle* p = &puzzle;
// Coms coms(p);

// void setup()
// {
//     WiFi.onEvent(coms.Event);
// }

// //coms.h
// class Coms
// {
//     public: 
//         static bool eth_connected;
//         static void Event(WiFiEvent_t event);
// }

// //coms.cpp
// void Coms::Event(WiFiEvent_t event)
// {
//     switch (event)
//     {
//     case SYSTEM_EVENT_ETH_START:
//         Serial.println("ETH Started");
//         //set eth hostname here
//         ETH.setHostname("esp32-ethernet");
//         break;
//     case SYSTEM_EVENT_ETH_CONNECTED:
//         Serial.println("ETH Connected");
//         break;
//     case SYSTEM_EVENT_ETH_GOT_IP:
//         Serial.print("ETH MAC: ");
//         Serial.print(ETH.macAddress());
//         Serial.print(", IPv4: ");
//         Serial.print(ETH.localIP());
//         if (ETH.fullDuplex())
//         {
//             Serial.print(", FULL_DUPLEX");
//         }
//         Serial.print(", ");
//         Serial.print(ETH.linkSpeed());
//         Serial.println("Mbps");
//         //TCP::eth_connected = true;
//         break;
//     case SYSTEM_EVENT_ETH_DISCONNECTED:
//         Serial.println("ETH Disconnected");
//         //TCP::eth_connected = false;
//         break;
//     case SYSTEM_EVENT_ETH_STOP:
//         Serial.println("ETH Stopped");
//         //TCP::eth_connected = false;
//         break;
//     default:
//         break;
//     }
// }