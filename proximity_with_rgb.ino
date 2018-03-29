/*
 Project:    IR Proximity
 Created:  22-11-2017 
 Author:  Dhruv Tripathi
 Modified by manoj Adding RGB LED With Switch
     On  : 28-03-2018 
*/


#include <IRremote.h>
#include <TimerOne.h>

#define IR_PIN 3
#define SENSOR_PIN 7
#define LEDOUT_PIN 6

const int brightness[32] = {
    0, 1, 2, 3, 4, 5, 7, 9, 12,
    15, 18, 22, 27, 32, 38, 44, 51, 58,
    67, 76, 86, 96, 108, 120, 134, 148, 163,
    180, 197, 216, 235, 255};

bool ledState = false;
int counter = 0;
int proximityVal = 0;
unsigned long timerTime = 1000000;

//+++++++++++++++++++++++++++++

const int ledPinR = 9;   // LED1 Cathod , Anode to Vcc via Resistance
const int ledPinG = 10;  // LED2 ANODE
const int ledPinB = 11;  // LED3 ANODE
const int mode1Pin = A5; // Input Pin pulldown by 4.7K, and switch with Vcc

int mode1 = 1;      // Selector State (Initial state = ALL OFF)
int val1 = 0;       // Pin 13 HIGH/LOW Status
int butState = 0;   // Last Button State
int mode1State = 0; // Last Mode State
boolean debug = 1;  // 1 = Print Serial Enabled / 0 = disabled

//+++++++++++++++++++++++++++++

IRsend irsend;

void setup()
{
    pinMode(IR_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);
    pinMode(LEDOUT_PIN, OUTPUT);
    digitalWrite(LEDOUT_PIN, LOW);
    //irsend.enableIROut(38);
    irsend.enableIROut(36);
    TIMER_ENABLE_PWM; // Enable pin 3 PWM output

    Timer1.initialize(timerTime);
    Timer1.attachInterrupt(switch_led_off);

    //++++++++++++++++++++++++++++++++++
    pinMode(ledPinR, OUTPUT);
    pinMode(ledPinG, OUTPUT);
    pinMode(ledPinB, OUTPUT);
    pinMode(mode1Pin, INPUT);
    if (debug)
    {
        Serial.begin(9600);
        Serial.print("Initial Mode: ");
        Serial.println(mode1);
        Serial.print("Setup Complete\n");
    }
    //++++++++++++++++++++++++++++++++++
}

void loop()
{
    proximityVal = digitalRead(SENSOR_PIN);
    if (proximityVal == 0)
    {
        switch_led();
    }
    //+++++++++++++++++++++++++++++++++++++

    val1 = digitalRead(mode1Pin);

    // If we see a change in button state, increment mode value
    if (val1 != butState && val1 == LOW)
    {
        mode1++;
        if (debug)
        {
            Serial.print("mode1 - ");
            Serial.println(mode1);
        }
    }

    butState = val1; // Keep track of most recent button state

    // No need to keep setting pins *every* loop
    if (mode1State != mode1)
    {

        // If no keys have been pressed yet don't execute
        // the switch code below
        // if (mode != 0) {

        switch (mode1)
        {
            //case 1 is actually handled below as default

        case 2:
            digitalWrite(ledPinR, HIGH);
            digitalWrite(ledPinG, LOW);
            digitalWrite(ledPinB, LOW);
            if (debug)
            {
                Serial.println(" RED ON ");
            }
            showState();
            break;
        case 3:
            digitalWrite(ledPinR, LOW);
            digitalWrite(ledPinG, HIGH);
            digitalWrite(ledPinB, LOW);
            if (debug)
            {
                Serial.println(" GREEN ON ");
            }
            showState();
            break;
        case 4:
            digitalWrite(ledPinR, LOW);
            digitalWrite(ledPinG, LOW);
            digitalWrite(ledPinB, HIGH);
            if (debug)
            {
                Serial.println(" BLUE ON ");
            }
            showState();
            break;
        case 5:
            digitalWrite(ledPinR, HIGH);
            digitalWrite(ledPinG, HIGH);
            digitalWrite(ledPinB, LOW);
            if (debug)
            {
                Serial.println(" YELLOW ON ");
            }
            showState();
            break;
        case 6:
            digitalWrite(ledPinR, HIGH);
            digitalWrite(ledPinG, LOW);
            digitalWrite(ledPinB, HIGH);
            if (debug)
            {
                Serial.println(" MEGENTA ON ");
            }
            showState();
            break;
        case 7:
            digitalWrite(ledPinR, LOW);
            digitalWrite(ledPinG, HIGH);
            digitalWrite(ledPinB, HIGH);
            if (debug)
            {
                Serial.println(" CYAN ON ");
            }
            showState();
            break;
        case 8:
            digitalWrite(ledPinR, HIGH);
            digitalWrite(ledPinG, HIGH);
            digitalWrite(ledPinB, HIGH);
            if (debug)
            {
                Serial.println(" WHITE ON ");
            }
            showState();
            break;
        default:
            mode1 = 1;
            // loop back to 1 by default, seems redundant but
            // it also handles the "mode is > 3" problem
            digitalWrite(ledPinR, LOW);
            digitalWrite(ledPinG, LOW);
            digitalWrite(ledPinB, LOW);
            if (debug)
            {
                Serial.println(" ALL Off ");
            }
            showState();
            break;
        } // end switch
          // } // end of "if mode = 0" check
    }     // end of ModeState check

    mode1State = mode1; // Keep track of mode recent mode value

    //+++++++++++++++++++++++++++++++++++++++

    delay(10); // slow the loop just a bit for debounce
}

void showState()
{
    if (debug)
    {
        Serial.print("Mode1: ");
        Serial.println(mode1);
        delay(100);
    }
}

void switch_led()
{ // To create Toggle function
    ledState = !ledState;
    if (ledState)
    {
        for (int i = 0; i < 32; i++)
        {
            analogWrite(LEDOUT_PIN, brightness[i]);
            delay(70);
        }
        Serial.print("LED - ");
        Serial.println("ON");
    }
    else
    {
        for (int i = 31; i >= 0; i--)
        {
            analogWrite(LEDOUT_PIN, brightness[i]);
            delay(70);
        }
        Serial.print("LED - ");
        Serial.println("OFF");
    }
    //digitalWrite(LEDOUT_PIN, digitalRead(LEDOUT_PIN) ^ 1);
    delay(1000);
}

void switch_led_off()
{ //  Timer function for 1 sec update
    counter++;
    int time = 20 * 60; // for 20 min
    if (counter == time)
    {
        digitalWrite(LEDOUT_PIN, LOW);
        counter = 0;
    }
}