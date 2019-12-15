#include <Arduino.h>
#include <Puzzle.h>


void Puzzle::Loop()
{   
    if (audio.available()) {
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
  
  if (!audio.begin(Serial2)) {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); 
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

int Puzzle::CheckStatus()
{
    int state = audio.readState();
    Serial.print("State: "); //read mp3 state
    Serial.println(state); //read mp3 state
    return state;
}

void Puzzle::PlaySong(byte num) {
    Serial.print("Playing song: ");
    Serial.println(num);
    audio.play(num);  
}

void Puzzle::printDetail(uint8_t type, int value){  
  String eventMessage;
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      eventMessage = "Time Out!";
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      eventMessage = "Stack Wrong!";
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      eventMessage = "Card Inserted!";
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      eventMessage = "Card Removed!";
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
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          eventMessage = "Card not found";
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          eventMessage = "Sleeping";
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          eventMessage = "Get Wrong Stack";
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          eventMessage = "Check Sum Not Match";
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          eventMessage = "File Index Out of Bound";
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          eventMessage = "Cannot Find File";
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          eventMessage = "In Advertise";
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }  
  mqtt->publish(clientId + "/Event", eventMessage, true, 1);
}

