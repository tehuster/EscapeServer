#include <Arduino.h>
#include <Puzzle.h>

#define SHORTKNOCK 0
#define LONGKNOCK 1
#define STARTKNOCK 2
#define END 3

unsigned long previousMillis = 0; // will store last time LED was updated
unsigned long currentMillis = 0;
const long interval = 10; // interval at which to blink (milliseconds)

bool knocked = false;
bool debounce = false;
bool completed = false;

int phase = 0;

int counter = 0;
int debounceTresshold = 10;

int shortKnockTresshold = 45;
int longKnockTresshold = 160;

int knockTarget[3][6] = {
    {STARTKNOCK, SHORTKNOCK, SHORTKNOCK, END, END, END},
    {STARTKNOCK, SHORTKNOCK, SHORTKNOCK, SHORTKNOCK, SHORTKNOCK, END},
    {STARTKNOCK, LONGKNOCK, LONGKNOCK, SHORTKNOCK, SHORTKNOCK, END}};

int knockSequence[32];
int knockIndex = 0;

void Puzzle::Loop()
{
    Knocker();

    // // uint8_t sit_status_1 = IO_1->digitalRead(SIT_SENSOR_1);
    // // uint8_t sit_status_2 = !IO_1->digitalRead(SIT_SENSOR_2);

    // // Serial.print("1: ");
    // // Serial.print(sit_status_1);
    // // Serial.print(" 2: ");
    // // Serial.println(sit_status_2);
}

void Puzzle::Knocker()
{
    if (completed)
    {
        return;
    };

    int knockValue = analogRead(4);

    if(knockValue  > 50){
        // Serial.println(knockValue);
    }    

    if (knockValue > 100 && debounce)
    {
        if (knockIndex == 0)
        {
            Serial.println("STARTKNOCK");
            knockSequence[knockIndex] = STARTKNOCK;
            knockIndex++;
        }
        else
        {
            if (counter < shortKnockTresshold)
            {
                Serial.println("SHORTKNOCK");
                knockSequence[knockIndex] = SHORTKNOCK;
                knockIndex++;
            }
            else if (counter < longKnockTresshold)
            {
                Serial.println("LONGKNOCK");
                knockSequence[knockIndex] = LONGKNOCK;
                knockIndex++;
            }
        }
        if(knockIndex > 31)
        {
            knockIndex = 31;
        }
        counter = 0;
        debounce = false;
    }

    currentMillis = millis();

    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        counter++;

        if (counter > longKnockTresshold)
        {
            if (knockIndex > 0)
            {
                if (CheckTarget())
                {
                    phase++;
                    String eventMessage = "Completed: " + String(phase);
                    String progressMessage = String(phase) + "/33";
                    Serial.println(eventMessage);                    
                    mqtt->publish(clientId + "/Event", eventMessage, true, 1);
                    mqtt->publish(clientId + "/Progress", progressMessage, true, 1);                       
                    if (phase > 2)
                    {                       
                        completed = true;
                    }
                }
            }
            counter = 0;
            knockIndex = 0;
        }

        if (counter > debounceTresshold)
        {
            debounce = true;
        }
    }
}

bool Puzzle::CheckTarget()
{
    bool correctSequence = true;
    int knockTargetAmount = 0;

    for (int i = 0; i < 32; i++)
    {
        if (knockTarget[phase][i] == END)
        {
            break;
        }

        if (knockTarget[phase][i] != knockSequence[i])
        {
            Serial.println("Incorrect knocks");
            correctSequence = false;
        }
        knockSequence[i] = 0;
        knockTargetAmount++;
    }

    if (knockIndex > knockTargetAmount || knockIndex < knockTargetAmount)
    {
        Serial.print(knockIndex);
        Serial.println(" : Too many/few knocks");
        Serial.print("Correct Amount shoud be: ");
        Serial.println(knockTargetAmount);
        correctSequence = false;
    }

    return correctSequence;
}

void Puzzle::Reset()
{
    phase = 0;
    completed = false;

    Serial.println("Resetting puzzle");
    IO_RELAYBOARD->digitalWrite(DOOR_1, HIGH);
    IO_RELAYBOARD->digitalWrite(DOOR_2, HIGH);
    IO_RELAYBOARD->digitalWrite(KNOCKER, LOW);
    IO_RELAYBOARD->digitalWrite(LIGHT_REBUS, LOW);
    IO_2->digitalWrite(LIGHT_1, LOW);
    IO_2->digitalWrite(LIGHT_2, LOW);
    IO_2->digitalWrite(LIGHT_3, LOW);
}

void Puzzle::LoadIO(MCP *_IO_1, MCP *_IO_2)
{
    IO_RELAYBOARD = _IO_1;
    IO_2 = _IO_2; 
}

void Puzzle::LoadPuzzle(Preferences p)
{
    preferences = p;
    // preferences.begin("puzzle", false);
    // open = preferences.getUInt("open", 0);
    // closed = preferences.getUInt("closed", 0);
    // Serial.print("Loaded [open] : ");
    // Serial.println(open);
    // Serial.print("Loaded [closed] : ");
    // Serial.println(closed);
    // preferences.end();    

    
    pinMode(4, INPUT);
}

void Puzzle::LoadMQTT(MQTTClient *mqttClient, String _clientId)
{
    clientId = _clientId;
    mqtt = mqttClient;
}

void Puzzle::TurnOffLight(uint8_t lamp)
{
    if (lamp == 4)
    {
        IO_RELAYBOARD->digitalWrite(lamp, LOW);
    }
    else if (lamp < 4)
    {
        IO_2->digitalWrite(lamp, LOW);
    }
}

void Puzzle::TurnOnLight(uint8_t lamp)
{
    if (lamp == 4)
    {  
        IO_RELAYBOARD->digitalWrite(lamp, HIGH);
    }
    else if (lamp < 4)
    {  
        IO_2->digitalWrite(lamp, HIGH);
    }
}

void Puzzle::OpenDoor(uint8_t door)
{
    IO_RELAYBOARD->digitalWrite(door, LOW);
}

void Puzzle::CloseDoor(uint8_t door)
{
    IO_RELAYBOARD->digitalWrite(door, HIGH);
}

void Puzzle::KnockHint(uint8_t knockAmount)
{
    // for (int i = 0; i < knockAmount; i++)
    // {
    //     int knockInterval = 250;
    //     IO_1->digitalWrite(1, HIGH);
    //     delay(knockInterval);
    //     IO_1->digitalWrite(1, LOW);
    //     delay(knockInterval);
    // }
}

// void Puzzle::SetOpenValue(int value)
// {
//     open = value;

//     preferences.begin("puzzle", false);
//     preferences.putUInt("open", open);
//     preferences.end();

//     Serial.print("Set [open] : ");
//     Serial.println(open);
// }

// void Puzzle::SetClosedValue(int value)
// {
//     closed = value;

//     preferences.begin("puzzle", false);
//     preferences.putUInt("closed", closed);
//     preferences.end();

//     Serial.print("Set [closed] : ");
//     Serial.println(closed);
// }

// int Puzzle::GetOpenValue()
// {
//   int value = open;
//   return value;
// }

// int Puzzle::GetClosedValue()
// {
//   int value = closed;
//   return value;
// }