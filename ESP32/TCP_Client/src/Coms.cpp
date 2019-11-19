#include <Coms.h>
#include <Puzzle.h>

void Coms::Status()
{
    //Serial.println("RequestType: Status");
}

void Coms::Data()
{
    Serial.println("RequestType: Data");
}

void Coms::Action()
{
    Serial.println("RequestType: Action");
    String actionName = request["actionName"];
    String actionValue = request["actionParameter"];

    if (actionName == "BlinkLed")
    {
        puzzle->BlinkLed();
    }
    else
    {
        Serial.print("Unknown: ActionName: ");
        Serial.println(actionName);
    }
}

void Coms::Config_set()
{
    Serial.println("RequestType: Config_set");
    String variableName = request["actionName"];
    String variableValue = request["actionParameter"];

    if (variableName == "blinkTime")
    {
        puzzle->setBlinkTime(variableValue.toInt());
        Serial.print("blinkTime: ");
        Serial.println(puzzle->getBlinkTime());        
    }
    else
    {
        Serial.print("Unknown: VariableName: ");
        Serial.println(variableName);
    }
}

void Coms::Config_get()
{   
    Serial.println("RequestType: Config_get");
    String variableName = request["actionName"];
    String variableValue = request["actionParameter"];

    if (variableName == "blinkTime")
    {
        Serial.print("blinkTime: ");
        Serial.println(puzzle->getBlinkTime());
    }
    else
    {
        Serial.print("Unknown: VariableName: ");
        Serial.println(variableName);
    }
}

void Coms::handleRequest()
{
    request = parseJson(messageRX);

    String rTypeString = request["requestType"];
    //#include <map>
    //std::map<T, T> key value pair dictionairy voor generieke toekomst.

    //#include <unordered_map>
    //typedef void (*RequestFunc)(void); // function pointer type
    //typedef std::unordered_map<String, RequestFunc> funcMap;
    //funcMap requestFuncs;
    //requestFuncs.emplace("appel", &Status);
    
    if (rTypeString == "status")
    {
        Status();
    }else if (rTypeString == "data")
    {
        Data();
    }
    else if (rTypeString == "action")
    {
        Action();
    }
    else if (rTypeString == "config_get")
    {
        Config_get();
    }
    else if (rTypeString == "config_set")
    {
        Config_set();
    }
    else
    {
        Serial.println("Unknown: RequestType");
    }
}

void Coms::loadPreferences(Preferences &preferences)  //"&" pass by reference (memory address), not by value (copy of memory).
{
    //Serial.print("Getting preferences variables: ");
    preferences.begin("puzzle", false);
    puzzle->setBlinkTime(preferences.getUInt("blinkTime", 0));
    preferences.end();
    //Serial.println(puzzle.getBlinkTime());
}


void Coms::createStatus()
{
    JsonObject status = Response.createNestedObject("status");

    status["name"] = "Client101";
    status["requestType"] = "data";

    serializeJson(Response, jsonResponse);
}

StaticJsonDocument<255> Coms::parseJson(String json)
{   
    DeserializationError err = deserializeJson(Request, json);    
    
    if (err)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(err.c_str());  
    }    

    return Request;
}

void Coms::sendData(const char *data)
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

void Coms::Event(WiFiEvent_t event)
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