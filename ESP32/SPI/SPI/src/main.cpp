#include <Arduino.h>
#include <SPI.h>

#define SETLED 0
#define SHOWLEDS 1

void SetLed(int ledIndex, int r, int g, int b);
void ShowLeds();

void setup()
{
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    pinMode(32, OUTPUT);
    digitalWrite(32, HIGH); // disable Slave Select

    pinMode(34, INPUT);
    Serial.begin(115200);   //set baud rate to 115200 for usart    

    //10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK)
    //32     , 15       , 16         14
    SPI.begin(14, 16, 15, 32);
    SPI.setClockDivider(SPI_CLOCK_DIV32);
}

void loop()
{
    int buttonPress = digitalRead(34);
    
    if(buttonPress)
    {
    for(int i = 0; i < 10; i++)
    {
        SetLed(i, 255, 0, 0);
        ShowLeds();
    }

    delay(10);

    for(int i = 0; i < 10; i++)
    {
        SetLed(i, 0, 255, 0);
        ShowLeds();
    }    
    delay(10);
    }
}


void SetLed(int ledIndex, int r, int g, int b)
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

void ShowLeds()
{
    digitalWrite(32, LOW); // enable Slave Select
    SPI.transfer(SHOWLEDS); 
    SPI.transfer('\n');   
    digitalWrite(32, HIGH); // disable Slave Select
    delay(1);               // Wait for Ledstrip, do we need this?
}