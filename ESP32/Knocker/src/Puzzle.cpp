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
    {STARTKNOCK, LONGKNOCK, SHORTKNOCK, LONGKNOCK, SHORTKNOCK, END},
    {STARTKNOCK, SHORTKNOCK, SHORTKNOCK, SHORTKNOCK, END, END},
    {STARTKNOCK, SHORTKNOCK, SHORTKNOCK, SHORTKNOCK, SHORTKNOCK, END}};

int knockSequence[32];
int knockIndex = 0;

void Puzzle::Loop()
{
    if (completed)
    {
        return;
    };

    int knockValue = analogRead(4);

    if (knockValue > 500 && debounce)
    {
        if (knockIndex == 0)
        {
            knockSequence[knockIndex] = STARTKNOCK;
            knockIndex++;
        }
        else
        {
            if (counter < shortKnockTresshold)
            {
                knockSequence[knockIndex] = SHORTKNOCK;
                knockIndex++;
            }
            else if (counter < longKnockTresshold)
            {
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

    // // uint8_t sit_status_1 = IO_1->digitalRead(SIT_SENSOR_1);
    // // uint8_t sit_status_2 = !IO_1->digitalRead(SIT_SENSOR_2);

    // // Serial.print("1: ");
    // // Serial.print(sit_status_1);
    // // Serial.print(" 2: ");
    // // Serial.println(sit_status_2);
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
        Serial.println("Too many/few knocks");
        Serial.print("Correct Amount shoud be: ");
        Serial.println(knockTargetAmount);
        correctSequence = false;
    }

    return correctSequence;
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
    // preferences.begin("puzzle", false);
    // open = preferences.getUInt("open", 0);
    // closed = preferences.getUInt("closed", 0);
    // Serial.print("Loaded [open] : ");
    // Serial.println(open);
    // Serial.print("Loaded [closed] : ");
    // Serial.println(closed);
    // preferences.end();

    //TURN OF CHIPSELECT FOR IO EXPANDER
    pinMode(IO_1_SS, OUTPUT);
    digitalWrite(IO_1_SS, HIGH);

    pinMode(IO_2_SS, OUTPUT);
    digitalWrite(IO_2_SS, HIGH);

    pinMode(34, INPUT);

    IO_1 = new MCP(0, IO_2_SS);
    IO_2 = new MCP(0, IO_1_SS);

    IO_1->begin();
    IO_1->pinMode(DOOR_1, OUTPUT);
    IO_1->pinMode(DOOR_2, OUTPUT);
    IO_1->pinMode(KNOCKER, OUTPUT);
    IO_1->pinMode(LIGHT_REBUS, OUTPUT);

    IO_1->pinMode(SIT_SENSOR_1, INPUT);
    IO_1->pullupMode(SIT_SENSOR_1, HIGH);
    IO_1->pinMode(SIT_SENSOR_2, INPUT);
    IO_1->pullupMode(SIT_SENSOR_2, HIGH);

    IO_2->begin();
    IO_2->pinMode(LIGHT_1, OUTPUT);
    IO_2->pinMode(LIGHT_2, OUTPUT);
    IO_2->pinMode(LIGHT_3, OUTPUT);

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
        IO_1->digitalWrite(lamp, LOW);
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
        IO_1->digitalWrite(lamp, HIGH);
    }
    else if (lamp < 4)
    {
        IO_2->digitalWrite(lamp, HIGH);
    }
}

void Puzzle::OpenDoor(uint8_t door)
{
    IO_1->digitalWrite(door, LOW);
}

void Puzzle::CloseDoor(uint8_t door)
{
    IO_1->digitalWrite(door, HIGH);
}

void Puzzle::KnockHint(uint8_t knockAmount)
{
    for (int i = 0; i < knockAmount; i++)
    {
        int knockInterval = random(100, 500);
        IO_1->digitalWrite(KNOCKER, HIGH);
        delay(knockInterval);
        IO_1->digitalWrite(KNOCKER, LOW);
        delay(knockInterval);
    }
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