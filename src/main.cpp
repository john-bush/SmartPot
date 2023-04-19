#include "main.h"

float p = 3.1415926;

Interface ui = Interface();

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Initializing");
    InitEncoder();
    ui.Init();
    //ui.LoadingScreen();
    ui.ClearDisplay();




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
    int a = digitalRead(ENCODER_PIN_A);
    int b = digitalRead(ENCODER_PIN_B);

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
    buttonState = digitalRead(BUTTON_PIN);

    // Check for button press with debounce
    if (buttonState != lastButtonState)
    {
        if ((millis() - lastDebounceTime) > debounceDelay)
        {
            if (buttonState == LOW)
            {
                // call UI update function
                Serial.println("Button Pressed");
            }
            lastDebounceTime = millis();
        }
        lastButtonState = buttonState;
    }
}

void InitEncoder()
{
    // Initialize pins as input/output

    PCICR |= _BV(PCIE2); //enable interrupts on pin changes for reg D
	PCMSK2 |= _BV(PCINT2) |  _BV(PCINT3) | _BV(PCINT4); // mask for interrupts on pins D2,D3,D4 (pins 4,5,6)
    sei(); // Enable Global Interrupts

    pinMode(ENCODER_PIN_A, INPUT);
    pinMode(ENCODER_PIN_B, INPUT);
    pinMode(BUTTON_PIN, INPUT);

}


ISR(PCINT2_vect)
{

  //Serial.println("Interrupt");
  buttonISR();
  encoderISR();


}
