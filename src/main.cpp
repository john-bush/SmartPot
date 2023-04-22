#include "main.h"
#include "i2c.h"
#include "tsl2591.h"
#include "dht20.h"

#include "analog.h"

#define DEBUG 1

float p = 3.1415926;

Interface ui = Interface();

#ifndef DEBUG
    DHT20 dht = DHT20();
    TSL2591 tsl = TSL2591();
#endif

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Initializing");

    ui.Init();
    //ui.LoadingScreen();

#ifndef DEBUG
    InitEncoder();
    i2c_init(BDIV);
#endif

    ui.ClearDisplay();
    Serial.begin(9600);

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

#ifndef DEBUG
    uint32_t luminosity = tsl.rd_luminosity();
    dht.full_measurement();
    float humidity = dht.get_humidity();
    float temperature = dht.get_temperature();

    Serial.printf("Luminosity: %d \n", luminosity);
    Serial.printf("Humidity: %d \n", humidity);
    Serial.printf("Temperature: %d \n", temperature);
#endif



    delay(2000);
    ui.UpdateTank(0, 0);

    delay(2000);

    
    ui.DrawPlantSelectionScreen();
    delay(2000);
    ui.ScrollForward();
    delay(2000);
    ui.ScrollForward();
    delay(2000);
    ui.ScrollForward();
    delay(2000);
    ui.ScrollBackward();

    ui.DrawPlantConfirmationScreen();
    delay(2000);
    ui.ScrollForward();
    delay(2000);
    ui.ScrollForward();
    delay(2000);
    ui.ScrollForward();
    delay(2000);
    ui.ScrollBackward();
    delay(5000);

    ui.DrawPlantDashboard();
    

    delay(2000);
    ui.SetTank(1, 0);
    ui.UpdatePlantDashboard();

    delay(2000);
    ui.SetTank(1, 1);
    ui.UpdatePlantDashboard();

    delay(2000);
    ui.SetTank(0, 1);
    ui.UpdatePlantDashboard();
}

void encoderISR()
{
    //check if the interrupt is a button change
    buttonState = digitalRead(BUTTON_PIN);
    if(buttonState != lastButtonState){
        return;
    }


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
            if(encoderCount %4 ==0){
                ui.ScrollForward();
            }
        }
        else
        {
            encoderCount--;
            if(encoderCount %4 ==0){
                ui.ScrollBackward();
            }
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
