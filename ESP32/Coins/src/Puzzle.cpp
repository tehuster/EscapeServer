#include <Arduino.h>
#include <Puzzle.h>

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 100;           // interval at which to blink (milliseconds)
bool newTag = false;
uint8_t counter = 0;
uint8_t resetTime = 10;

void Puzzle::Loop()
{
  success = rfid->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30);
  if (success)
  {    
    String rfidTag;
    for (int i = 0; i < 6; i++)
    {
      rfidTag += String(uid[i]);
    }

    if(rfidTag != old_rfidTag)
    {
      Serial.println(rfidTag);
      mqtt->publish(clientId + "/Event", rfidTag, true, 1);
      Toggle();
      newTag = true;
      counter = 0;
    }
    old_rfidTag = rfidTag;    
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {      
      previousMillis = currentMillis;
      if(newTag)
      {
        counter ++;
        if(counter > resetTime)
        {
          old_rfidTag = "";
          newTag = false;
        }
      }
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
  open = preferences.getUInt("open", 0);
  closed = preferences.getUInt("closed", 0);
  Serial.print("Loaded [open] : ");
  Serial.println(open);
  Serial.print("Loaded [closed] : ");
  Serial.println(closed);
  preferences.end();

  //TURN OF CHIPSELECT FOR IO EXPANDER
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH);

  pinMode(34, INPUT);

  ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
  ledcAttachPin(4, 1);   // GPIO 22 assigned to channel 1

  rfid = new Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
  rfid->begin();
  rfid->SAMConfig();
  Serial.println("Init RFIDReader ");
}

void Puzzle::LoadMQTT(MQTTClient *mqttClient, String _clientId)
{
  clientId = _clientId;
  mqtt = mqttClient;
}

void Puzzle::Open()
{
  ledcWrite(1, open);
}
void Puzzle::Close()
{  
  ledcWrite(1, closed);
}
void Puzzle::Toggle()
{
  ledcWrite(1, open);
  delay(1000);
  ledcWrite(1, closed);
}  

void Puzzle::SetOpenValue(int value)
{
    open = value;

    preferences.begin("puzzle", false);
    preferences.putUInt("open", open);
    preferences.end();

    Serial.print("Set [open] : ");
    Serial.println(open);    
}

void Puzzle::SetClosedValue(int value)
{
    closed = value;

    preferences.begin("puzzle", false);
    preferences.putUInt("closed", closed);
    preferences.end();

    Serial.print("Set [closed] : ");
    Serial.println(closed);   
}

int Puzzle::GetOpenValue()
{
  int value = open;
  return value;
} 

int Puzzle::GetClosedValue()
{  
  int value = closed;
  return value;
}