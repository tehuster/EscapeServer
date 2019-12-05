// ///////////////////// UTILITY ///////////////////

// String GetValue(String data, char separator, int index)
// {
//   int found = 0;
//   int strIndex[] = {0, -1};
//   int maxIndex = data.length() - 1;

//   for (int i = 0; i <= maxIndex && found <= index; i++)
//   {
//     if (data.charAt(i) == separator || i == maxIndex)
//     {
//       found++;
//       strIndex[0] = strIndex[1] + 1;
//       strIndex[1] = (i == maxIndex) ? i + 1 : i;
//     }
//   }
//   return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
// }

// ///////////////////// MQTT ///////////////////

// void MQTT_Update()
// {
//   if (eth_connected)
//   {
//     if (!mqtt.connected())
//     {
//       Connect();
//     }
//     else
//     {
//       mqtt.loop();
//     }
//   }
//   int buttonPress = digitalRead(34);
//   if (buttonPress)
//   {
//     mqtt.publish(clientId + "/Event", "Button is pressed!", true, 1);
//     delay(1000);
//   }
// }

// void MessageReceived(String &topic, String &payload)
// {
//   Serial.println("incoming: " + topic + " - " + payload);

//   digitalWrite(2, HIGH); // Turn the LED on (Note that LOW is the voltage level

//   if (topic == clientId + "/Data")
//   {
//     HandleData(payload);
//   }
//   else if (topic == clientId + "/Action")
//   {
//     HandleAction(payload);
//   }
//   else if (topic == clientId + "/Get")
//   {
//     HandleGet(payload);
//   }
//   else if (topic == clientId + "/Set")
//   {
//     HandleSet(payload);
//   }

//   digitalWrite(2, LOW); // Turn the LED off by making the voltage HIGH
// }

// void Connect()
// {
//   while (!mqtt.connected())
//   {
//     Serial.print("Attempting MQTT connection...");
//     if (mqtt.connect(clientId.c_str()))
//     {
//       Serial.println("Connected with MQTT broker");
//       mqtt.publish(clientId + "/Event", "Connected", true, 1);

//       for (size_t i = 0; i < 4; i++)
//       {
//         String subName = clientId;
//         subName += "/";
//         subName += subscribers[i];
//         Serial.print("Subscribing to [");
//         Serial.print(subName);
//         Serial.println("]");
//         mqtt.subscribe(subName.c_str());
//       }
//     }
//     else
//     {
//       Serial.print("Failed to connect to broker");
//       Serial.println(" trying again in 2 seconds");
//       delay(2000);
//     }
//   }
// }

// void InitMQTT()
// {
//   WiFi.onEvent(WiFiEvent);
//   ETH.begin();

//   mqtt.begin(mqtt_server, ethernetConnection);
//   String willTopic = clientId + "/Error";
//   mqtt.setWill(willTopic.c_str(), "Device disconnected", true, 2);  //<-----------------
//   mqtt.setOptions(5, true, 777);
//   mqtt.onMessage(MessageReceived);
// }

// void WiFiEvent(WiFiEvent_t event)
// {
//   switch (event)
//   {
//   case SYSTEM_EVENT_ETH_START:
//     Serial.println("ETH Started");
//     ETH.setHostname(clientId.c_str());
//     break;
//   case SYSTEM_EVENT_ETH_CONNECTED:
//     Serial.println("ETH Connected");
//     break;
//   case SYSTEM_EVENT_ETH_GOT_IP:
//     Serial.print("ETH MAC: ");
//     Serial.print(ETH.macAddress());
//     Serial.print(", IPv4: ");
//     Serial.print(ETH.localIP());
//     if (ETH.fullDuplex())
//     {
//       Serial.print(", FULL_DUPLEX");
//     }
//     Serial.print(", ");
//     Serial.print(ETH.linkSpeed());
//     Serial.println("Mbps");
//     eth_connected = true;
//     break;
//   case SYSTEM_EVENT_ETH_DISCONNECTED:
//     Serial.println("ETH Disconnected");
//     eth_connected = false;
//     break;
//   case SYSTEM_EVENT_ETH_STOP:
//     Serial.println("ETH Stopped");
//     eth_connected = false;
//     break;
//   default:
//     break;
//   }
// }