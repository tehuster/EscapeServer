#include <Arduino.h>
#include <Puzzle.h>

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 10;           // interval at which to blink (milliseconds)
int counter = 0;
void Puzzle::Loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        // set the LED with the ledState of the variable:
        SetLed(counter, 64, 0, 0);
        ShowLeds();
        SetLed(counter, 0, 0, 0);
        ShowLeds();

         counter ++;
        if(counter >= NUM_LEDS)
        {
            counter = 0;
        }
    }
    
   
    // ShowName(Tiny, false);
    // delay(2500);
    // TurnOffLeds();
    // ShowName(Small, true);
    // delay(2500);
    // TurnOffLeds();
    // ShowName(Medium, true);
    // delay(2500);
    // TurnOffLeds();
    // ShowName(Big, true);
    // delay(2500);
    // TurnOffLeds();
    // ShowName(Huge, true);
    // delay(2500);    
    // TurnOffLeds();
}

void Puzzle::Reset()
{
    Serial.println("Resetting puzzle");
}

void Puzzle::LoadPuzzle(Preferences p)
{
    delay(1000);
    preferences = p;
    // preferences.begin("puzzle", false);
    // blinkTime = preferences.getUInt("blinkTime", 0);
    // Serial.print("Loaded [blinkTime] : ");
    // Serial.println(blinkTime);
    preferences.end();

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    pinMode(32, OUTPUT);
    digitalWrite(32, HIGH); 

    pinMode(34, INPUT);

    //10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK)
    //32     , 15       , 16         14
    SPI.begin(14, 16, 15, 32);
    SPI.setClockDivider(SPI_CLOCK_DIV32);
  
    // FastLED.setBrightness(BRIGHTNESS); TODO: Create setBrightness for SPI LED Slave
    TurnOffLeds();
}

void Puzzle::TurnOffLeds()
{
    for (int i = 0; i < ledAmount; i++)
    {
        SetLed(i, 0, 0, 0);
    }
    ShowLeds();
}

void Puzzle::ShowName(Name name, bool writeName)
{
    if(writeName)
    {
        FlowName(nameLedBegin[name], nameLedAmount[name], writeSpeed);
    }else
    {
        StampName(nameLedBegin[name], nameLedAmount[name]);
    }
    
}

void Puzzle::Hint()
{

}

void Puzzle::WriteName(int start, int length, int speed)
{
    for (int i = start; i < start + length; i++)
    {
        SetLed(i, 64, 0, 0);
        delay(speed);
        ShowLeds();
    }   
}

void Puzzle::FlowName(int start, int length, int speed)
{
    for (int i = start; i < start + length; i++)
    {
        for (int b = 0; b < 200; b++)
        {
            SetLed(i, 0, b, 0);
            delay(speed);
            ShowLeds();
        }   
    }   
}

void Puzzle::StampName(int start, int length)
{
    for (int i = start; i < start + length; i++)
    {
        SetLed(i, 64, 0, 0);       
    } 
    ShowLeds();
}   

void Puzzle::SetLed(int ledIndex, int r, int g, int b)
{
    digitalWrite(32, LOW); // enable Slave Select
    SPI.transfer(SETLED); 
    SPI.transfer(ledIndex);    
    SPI.transfer(r);    
    SPI.transfer(g); 
    SPI.transfer(b); 
    SPI.transfer('\n');   
    digitalWrite(32, HIGH); // disable Slave Select
    delay(1);               // Wait for Ledstrip, do we need this?
}

void Puzzle::ShowLeds()
{
    digitalWrite(32, LOW); // enable Slave Select
    SPI.transfer(SHOWLEDS); 
    SPI.transfer('\n');   
    digitalWrite(32, HIGH); // disable Slave Select
    delay(1);               // Wait for Ledstrip, do we need this?
}