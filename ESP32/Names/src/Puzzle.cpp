#include <Arduino.h>
#include <Puzzle.h>

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)

void Puzzle::Loop()
{
    int value = digitalRead(33);
    if(value == LOW)
    {
        // Serial.println("Slave is busy!");
    }else
    {
        if(bufferIndex != 0)
        {
            SendBuffer();
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
    preferences = p;
    preferences.begin("puzzle", false);

    brightness = preferences.getUInt("brightness", 0);
    Serial.print("Loaded [brightness] : ");
    Serial.println(brightness);

    writeSpeed = preferences.getUInt("writeSpeed", 0);
    Serial.print("Loaded [writeSpeed] : ");
    Serial.println(writeSpeed);

    preferences.end();

    //TURN OF CHIPSELECT FOR IO EXPANDER
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    pinMode(32, OUTPUT);
    digitalWrite(32, HIGH); 

    pinMode(34, INPUT);
    pinMode(33, INPUT);

    //10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK)
    //32     , 15       , 16         14
    SPI.begin(14, 16, 15, 32);
    SPI.setClockDivider(SPI_CLOCK_DIV32);
  
    SetBrightness(brightness); 
    SetWriteSpeed(writeSpeed); 
    TurnOffLeds();
}

void Puzzle::WriteBuffer(uint8_t c)
{
    rfidBuffer[bufferIndex] = c;
    bufferIndex ++;
}

void Puzzle::SendBuffer()
{
    Serial.println("SendingBuffer !!");
    // Serial.println(bufferIndex);     
    digitalWrite(32, LOW); // enable Slave Select    
    for(int i = 0; i < msgSize; i++)
    {   
        uint8_t index = (msgSize - i);
        uint8_t msg = rfidBuffer[bufferIndex - index];
        // Serial.println(msg);
        SPI.transfer(msg); 
    }
    bufferIndex -= msgSize;  
    digitalWrite(32, HIGH); // disable Slave Select   
    // Serial.println(bufferIndex);  
    // Serial.println("Done Sending!!");
}



void Puzzle::TurnOffLeds()
{   
    WriteBuffer(TURNOFFLEDS);
    WriteBuffer(TURNOFFLEDS);
    WriteBuffer(ENDMESSAGE);
    // PrintBuffer();
}

void Puzzle::ShowName(uint8_t name)
{
    WriteBuffer(SHOWNAME);
    WriteBuffer(name);
    WriteBuffer(ENDMESSAGE);
    // PrintBuffer();
}

void Puzzle::Hint(uint8_t type)
{
    WriteBuffer(HINT);
    WriteBuffer(type);
    WriteBuffer(ENDMESSAGE);
    // PrintBuffer();
}

void Puzzle::SetBrightness(uint8_t bright)
{
    brightness = bright;

    preferences.begin("puzzle", false);
    preferences.putUInt("brightness", brightness);
    preferences.end();

    Serial.print("Set [brightness] : ");
    Serial.println(brightness);   
    WriteBuffer(BRIGHTNESS);
    WriteBuffer(brightness);
    WriteBuffer(ENDMESSAGE);
    // PrintBuffer();
}

void Puzzle::SetWriteSpeed(uint8_t speed)
{
    writeSpeed = speed;

    preferences.begin("puzzle", false);
    preferences.putUInt("writeSpeed", writeSpeed);
    preferences.end();

    Serial.print("Set [writeSpeed] : ");
    Serial.println(writeSpeed);   

    WriteBuffer(WRITESPEED);
    WriteBuffer(writeSpeed);
    WriteBuffer(ENDMESSAGE);
    // PrintBuffer();
}

uint8_t Puzzle::GetBrightness()
{
    return brightness;
}

uint8_t Puzzle::GetWriteSpeed()
{
    return writeSpeed;
}

void Puzzle::SetColor_R(uint8_t value)
{
    WriteBuffer(COLOR_R);
    WriteBuffer(brightness);
    WriteBuffer(ENDMESSAGE);
    // PrintBuffer();
}

void Puzzle::SetColor_G(uint8_t value)
{
    WriteBuffer(COLOR_G);
    WriteBuffer(brightness);
    WriteBuffer(ENDMESSAGE);
    // PrintBuffer();
}

void Puzzle::SetColor_B(uint8_t value)
{
    WriteBuffer(COLOR_B);
    WriteBuffer(brightness);
    WriteBuffer(ENDMESSAGE);
    // PrintBuffer();
}

void Puzzle::PrintBuffer()
{
    Serial.print("BufferIndex: ");
    Serial.println(bufferIndex);
    for(int i = 0; i < bufferIndex; i++)
    {
        Serial.println(rfidBuffer[i]);
    }
}