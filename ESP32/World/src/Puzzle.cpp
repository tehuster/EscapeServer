#include <Arduino.h>
#include <Puzzle.h>

void Puzzle::Loop()
{

  //mqtt->publish(clientId + "/Event", rfidTag, true, 1);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    deviceCounter++;
    if (deviceCounter > 114)
    {
      deviceCounter = 101;
    }
    // Serial.print("..........");
    // Serial.println();
    // for (size_t i = 0; i < 14; i++)
    // {
    //     Serial.print(NodeSensorValue[i]);
    //     Serial.print(", ");
    // }
    // Serial.println();
    // for (size_t i = 0; i < 14; i++)
    // {
    //     Serial.print(NodeSensorInitialValue[i]);
    //     Serial.print(", ");
    // }
    // Serial.println();
    // for (size_t i = 0; i < 14; i++)
    // {
    //     Serial.print(NodeLedValues[i][0]);
    //     Serial.print(", ");
    // }
    // Serial.println();
    SendMessage(deviceCounter, NodeLedValues[deviceCounter - 101][0], NodeLedValues[deviceCounter - 101][1], NodeLedValues[deviceCounter - 101][2], 100);
  }

  for (size_t i = 0; i < 14; i++)
  {
      if(NodeSensorValue[i] > (NodeSensorInitialValue[i] + SensorTresshold) || NodeSensorValue[i] < (NodeSensorInitialValue[i] - SensorTresshold))
      {
          NodeLedValues[i][0] = 128;
          NodeLedValues[i][1] = 128;
          NodeLedValues[i][2] = 128;
      }
      else
      {
          NodeLedValues[i][0] = 0;
          NodeLedValues[i][1] = 0;
          NodeLedValues[i][2] = 0;
      }
      
  }

  ReceiveMessage();
}

void Puzzle::Reset()
{
  Serial.println("Resetting puzzle");
}

void Puzzle::LoadPuzzle(Preferences p)
{
  delay(1000);
  pinMode(DE, OUTPUT);
  Serial2.begin(bautrate, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Initialized Serial2");
  delay(1000);

  // preferences = p;
  // preferences.begin("puzzle", false);
  //preferences.end();
}

void Puzzle::LoadMQTT(MQTTClient *mqttClient, String _clientId)
{
  clientId = _clientId;
  mqtt = mqttClient;
}

void Puzzle::SendMessage(uint8_t adress, uint8_t R, uint8_t G, uint8_t B, uint8_t option)
{
  uint8_t SendArray[7];
  SendArray[0] = 255;    //start bute
  SendArray[1] = adress; //adress byte
  SendArray[2] = R;
  ; //red value
  SendArray[3] = G;
  ; //green value
  SendArray[4] = B;
  ;                      //blue value
  SendArray[5] = option; //option byte
  SendArray[6] = 254;    //end byte
  digitalWrite(DE, HIGH);
  Serial2.write(SendArray, sizeof(SendArray));
  delayMicroseconds((1000000 / bautrate) * 10 * sizeof(SendArray));
  digitalWrite(DE, LOW);
}

void Puzzle::ReceiveMessage()
{
  if (Serial2.available() == 7)
  {
    incomming_byte_counter = 0;
    while (Serial2.available())
    {
      incomingByte = Serial2.read();
      ReceiveArray[incomming_byte_counter] = incomingByte;
      incomming_byte_counter++;
    }

    if(NodeSensorInitialValue[ReceiveArray[1] - 101] == 0)
    {
      NodeSensorInitialValue[ReceiveArray[1] - 101] = ReceiveArray[5];
    }

    NodeSensorValue[ReceiveArray[1] - 101] = ReceiveArray[5];
    // uint8_t NodeSensorInitialValue[7]; 
    // uint8_t NodeSensorValue[7];    
    // uint8_t NodeLedValues[7][3]; 
    // SendMessage(ReceiveArray[1], ReceiveArray[5], ReceiveArray[5], ReceiveArray[5], 100);

    // for (int i = 0; i < 7; i++)
    // {
    //   Serial.print(i);
    //   Serial.print(": ");
    //   Serial.println(ReceiveArray[i]);
    // }
  }
}