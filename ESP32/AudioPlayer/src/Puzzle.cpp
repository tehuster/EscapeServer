#include <Arduino.h>
#include <Puzzle.h>

void Puzzle::Loop()
{
  if (audio.available())
  {
    printDetail(audio.readType(), audio.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
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
  preferences.begin("puzzle", false);
  volume = preferences.getUInt("volume", 0);
  Serial.print("Loaded [volume] : ");
  Serial.println(volume);
  preferences.end();

  InitAudio();
}

void Puzzle::LoadMQTT(MQTTClient *mqttClient, String _clientId)
{
  clientId = _clientId;
  mqtt = mqttClient;
}

void Puzzle::InitAudio()
{
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!audio.begin(Serial2, true, false))
  {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Recheck the connection."));
    Serial.println(F("2.Insert the SD card."));
    while (true)
    {
      delay(0); // the ESP8266 watchdog likes this.
    }
  }

  Serial.println(F("DFPlayer Mini online."));
  audio.setTimeOut(500);
  audio.volume(20);
  audio.play(2);
}

void Puzzle::SetVolume(int newVolume)
{
  volume = newVolume;
  preferences.begin("puzzle", false);
  preferences.putUInt("volume", volume);
  preferences.end();
  Serial.print("Set volume to: ");
  Serial.println(volume);
  audio.volume(volume);
}

String Puzzle::CheckStatus()
{
  int state = audio.readState();
  Serial.print("State: "); //read mp3 state
  Serial.println(state);   //read mp3 state
  String stateMsg;
  switch (state)
  {
  case 512:
    stateMsg = "Awesome";
    break;
  case 513:
    stateMsg = "Playing audio";
    break; 
   case -1:
    stateMsg = "Warning: NO SD CARD!";
    break;  
  default:
    stateMsg = "Warning: Unknown state!";
    break;
  }
  return stateMsg;
}

void Puzzle::PlayAudio(byte num)
{
  Serial.print("Playing song: ");
  Serial.println(num);
  audio.play(num);
  isPlaying = true;
}

void Puzzle::printDetail(uint8_t type, int value)
{
  String eventMessage;
  bool error = false;
  bool sendEvent = true;
  switch (type)
  {
  case TimeOut:
    Serial.println(F("Time Out!"));
    eventMessage = "Time Out!";
    break;
  case WrongStack:
    Serial.println(F("Stack Wrong!"));
    eventMessage = "Stack Wrong!";
    error = true;
    break;
  case DFPlayerCardInserted:
    Serial.println(F("Card Inserted!"));
    eventMessage = "Card Inserted!";
    break;
  case DFPlayerCardRemoved:
    Serial.println(F("Card Removed!"));
    eventMessage = "Card Removed!";
    error = true;
    break;
  case DFPlayerCardOnline:
    Serial.println(F("Card Online!"));
    eventMessage = "Card Online!";
    break;
  case DFPlayerUSBInserted:
    Serial.println("USB Inserted!");
    eventMessage = "USB Inserted!";
    break;
  case DFPlayerUSBRemoved:
    Serial.println("USB Removed!");
    eventMessage = "USB Removed!";
    break;
  case DFPlayerPlayFinished:
    Serial.print(F("Number:"));
    Serial.print(value);
    Serial.println(F(" Play Finished!"));
    eventMessage = "Number: ";
    eventMessage += value;
    eventMessage += " Finished playing!";
    isPlaying = false;
    sendEvent = false;
    break;
  case DFPlayerError:
    Serial.print(F("DFPlayerError:"));
    switch (value)
    {    
    case Busy:
      Serial.println(F("Card not found"));
      eventMessage = "Card not found";
      error = true;
      break;
    case Sleeping:
      Serial.println(F("Sleeping"));
      eventMessage = "Sleeping";
      error = true;
      break;
    case SerialWrongStack:
      Serial.println(F("Get Wrong Stack"));
      eventMessage = "Get Wrong Stack";
      error = true;
      break;
    case CheckSumNotMatch:
      Serial.println(F("Check Sum Not Match"));
      eventMessage = "Check Sum Not Match";
      error = true;
      break;
    case FileIndexOut:
      Serial.println(F("File Index Out of Bound"));
      eventMessage = "File Index Out of Bound";
      error = true;
      break;
    case FileMismatch:
      Serial.println(F("Cannot Find File"));
      eventMessage = "Cannot Find File";
      error = true;
      break;
    case Advertise:
      Serial.println(F("In Advertise"));
      eventMessage = "In Advertise";
      error = true;
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  if(!sendEvent)
  {
    return;
  }
  if(error)
  {
    mqtt->publish(clientId + "/Error", eventMessage, true, 1);
  }else
  {
    mqtt->publish(clientId + "/Event", eventMessage, true, 1);
  }  
}
