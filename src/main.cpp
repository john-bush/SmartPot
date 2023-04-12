#include "main.h"

float p = 3.1415926;

Interface ui = Interface();

void setup(void)
{
    InitEncoder();
    ui.Init();
    ui.LoadingScreen();

    delay(2000);

    // Color(0, 0, 255);
    // FillCircle(30);
    // TestChart();
}

void loop()
{
    ui.DrawTankScreen();

    delay(2000);
    ui.UpdateTank(0, 1);

    delay(2000);
    ui.UpdateTank(1, 1);

    delay(2000);
    ui.UpdateTank(0, 0);

    delay(2000);
    ui.DrawPlantSelectionScreen();

    ui.ScrollForward();
    delay(2000);
    ui.UpdatePlantSelectionScreen();
}

void encoderISR()
{
    // Read encoder state
    int a = digitalRead(encoderPinA);
    int b = digitalRead(encoderPinB);
    encoderState = (a << 1) | b;

    // Check for changes in encoder state
    if (encoderState != lastEncoderState)
    {
        if ((lastEncoderState == 0b00 && encoderState == 0b10) ||
            (lastEncoderState == 0b10 && encoderState == 0b11) ||
            (lastEncoderState == 0b11 && encoderState == 0b01) ||
            (lastEncoderState == 0b01 && encoderState == 0b00))
        {
            encoderCount++;
        }
        else
        {
            encoderCount--;
        }
        Serial.print("Encoder count: ");
        Serial.println(encoderCount);
        lastEncoderState = encoderState;
    }
}

void buttonISR()
{
    // Read button state
    buttonState = digitalRead(buttonPin);

    // Check for button press with debounce
    if (buttonState != lastButtonState)
    {
        if ((millis() - lastDebounceTime) > debounceDelay)
        {
            if (buttonState == LOW)
            {
                // call UI update function
            }
            lastDebounceTime = millis();
        }
        lastButtonState = buttonState;
    }
}

void InitEncoder()
{
    // Initialize pins as input/output
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);

    // Attach interrupts to encoder and button pins
    attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
}
