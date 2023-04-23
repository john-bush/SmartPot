#include "main.h"
#include "i2c.h"
#include "tsl2591.h"
#include "dht20.h"

#include "analog.h"

// #define DEBUG

float p = 3.1415926;

Interface ui = Interface();
DHT20 dht = DHT20();
TSL2591 tsl = TSL2591();

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Initializing");

    ui.Init();
    // ui.LoadingScreen();
    RetrievePastState();
    delay(500);

#ifndef DEBUG
    InitEncoder();
    i2c_init(BDIV);
    tsl.initialize();
#endif

}


void loop()
{
    int entryState = state;
    switch(state) {
        
        case 0: // Loading Screen
            if (firstLoop) {
                ui.LoadingScreen();
                firstLoop = false;
            }
            break;
        
        case 1: // Tank Screen
            if (firstLoop) {
                ui.DrawTankScreen();
                firstLoop = false;
            }

            PollSensors();
            ui.SetTank(waterTank, fertilizerTank);
            if (waterTank && fertilizerTank) {
                state = 2;
                firstLoop = true;
                ui.SetState(state);
            }
                
            break;
        case 2: // Plant Selection Screen
            if (firstLoop) {
                ui.DrawPlantSelectionScreen();
                firstLoop = false;
            }
            // *updates and state change are handled by the button press and scroll interrupts          
            break;
        case 3: // Plant Confirmation Screen
            if (firstLoop) {
                ui.DrawPlantConfirmationScreen();
                firstLoop = false;
            }
            break;
        case 4: // Plant Dashboard
            if (firstLoop) {
                readIntFromEEPROM(8, plantType); // get plant type from EEPROM
                ui.DrawPlantDashboard();
                firstLoop = false;
            }
            PollSensors();
            // send sensor data to Interface object
            ui.UpdateSensors(temperature, humidity, luminosity, waterTank, fertilizerTank);
            ui.UpdatePlantDashboard();
            break;
        default:
            break;
    }

    if (state != entryState) {
        firstLoop = true;
        writeIntIntoEEPROM(4, state);
        
    }
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
        #ifndef DEBUG
        Serial.print("Encoder count: ");
        Serial.println(encoderCount);
        #endif
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
                ui.ButtonPress();
                int oldState = state;
                state = ui.GetState(); // update state based on UI
                if (oldState != state) {
                    firstLoop = true;
                }
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

void PollSensors() 
{
    luminosity = tsl.rd_luminosity();
    dht.full_measurement();
    humidity = dht.get_humidity();
    temperature = dht.get_temperature();

    read_soil_moisture(soilMoisture);
    read_tank_levels(waterTank, fertilizerTank);

    Serial.printf("Luminosity: %d \n", luminosity);
    Serial.printf("Humidity: %d \n", humidity);
    Serial.printf("Temperature: %d \n", temperature);
}


void RetrievePastState()
{
    readBoolFromEEPROM(0, configured);
    readIntFromEEPROM(4, state);
    if (configured) {
        readIntFromEEPROM(8, plantType);
        ui.SetPlantType(plantType);
    }
}
