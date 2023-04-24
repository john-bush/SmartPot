#include "main.h"
#include "i2c.h"
#include "tsl2591.h"
#include "dht20.h"

#include "analog.h"

//#define DEBUG

float p = 3.1415926;

Interface ui = Interface();
DHT20 dht = DHT20();
TSL2591 tsl = TSL2591();

void setup(void)
{
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Initializing");
#endif
    ui.Init();
    ui.LoadingScreen();
    RetrievePastState();
    delay(500);

#ifndef DEBUG
    InitEncoder();
    init_pumps();
    i2c_init(BDIV);
    tsl.initialize();
#endif
    response1 = new char[12];
    response2 = new char[12];
    response3 = new char[12];
    memset(response1, '\0',12);
    memset(response2, '\0',12);
    memset(response3, '\0',12);
}


void loop()
{
    int entryState = state;
    stateChange = false;
    switch(state) {
        
        case 0: // Loading Screen
            if (stateChange) break;

            if (firstLoop ) {
                //ui.LoadingScreen();
                firstLoop = false;
            }
            break;
        
        case 1: // Tank Screen
            if (stateChange) break;
            
            if (firstLoop) {
                ui.DrawTankScreen();
                firstLoop = false;
            }

            PollSensors();
            ui.SetTank(waterTank, fertilizerTank);
            delay(500); // allows for screen refresh

            if (waterTank && fertilizerTank) {
                // prime water pumps
                turn_on_water_pump();
                delay(1000);
                turn_off_water_pump();
                delay(500);
                turn_on_fertilizer_pump();
                delay(1000);
                turn_off_fertilizer_pump();
                state = 2;
                firstLoop = true;
                ui.SetState(state);
                delay(2000);
            }
                
            break;
        case 2: // Plant Selection Screen
            if (stateChange) break;

            if (firstLoop) {
                ui.DrawPlantSelectionScreen();
                firstLoop = false;
            }
            // *updates and state change are handled by the button press and scroll interrupts          
            break;
        case 3: // Plant Confirmation Screen
            if (stateChange) break;

            if (firstLoop) {
                ui.DrawPlantConfirmationScreen();
                firstLoop = false;
            }
            break;
        case 4: // Plant Dashboard
            if (stateChange) break;

            if (firstLoop) {
                readIntFromEEPROM(8, plantType); // get plant type from EEPROM
                ui.DrawPlantDashboard();
                firstLoop = false;

                idealHumidity = 40 + 2 * plantType;
                idealLuminosity = 3000 - 100 * plantType;
                idealSoilMoisture = 550 - 30 * plantType;
                idealTemperature = 25 - plantType;
            }
            PollSensors();
            // send sensor data to Interface object
            PlantCare();
            ui.UpdateSensors(soilMoisture, soilMoistureIntegral, luminosity, waterTank, fertilizerTank);
            ui.UpdatePlantDashboard();
            break;
        default:
            state = 0;
            break;
    }

    if (state != entryState) {
        firstLoop = true;
        writeIntIntoEEPROM(4, state);
        
    }

    if (stateChange) {
        state = ui.ButtonPress();
        stateChange = false;
        firstLoop = true;
    }

}

char* PlantCare() {
    // *** CALCULATIONS *** //
    // temperature integral
    float adjustedTemperature = temperature; //- idealTemperature;
    temperatureIntegral += (adjustedTemperature + previousHumidity) * DT * 0.5;
    previousTemperature = adjustedTemperature;
    
    // humidity integral
    float adjustedHumidity = humidity - idealHumidity;
    humidityIntegral += (adjustedHumidity + previousHumidity) * DT * 0.5;
    previousHumidity = adjustedHumidity;
    
    // luminosity integral
    int adjustedLuminosity = luminosity - idealLuminosity;
    luminosityIntegral += (adjustedLuminosity + previousLuminosity) * DT * 0.5;
    previousLuminosity = adjustedLuminosity;

    // soil moisture integral
    int adjustedSoilMoisture = soilMoisture - idealSoilMoisture;
    soilMoistureIntegral += (adjustedSoilMoisture + previousSoilMoisture) * DT * 0.5;
    previousSoilMoisture = adjustedSoilMoisture;

    // *** CONTROL *** //
    if (!watering) { // check if we need to water the plant
        if (soilMoistureIntegral > soilMoistureThreshold || soilMoistureIntegral < -soilMoistureThreshold) {
            // signal to raise soil moisture of plant
            unsigned long curr_time = millis();
            // start watering
            if ((curr_time - lastFertilizer > fertilizerTimeout) && fertilizerTank) { // check if plant needs fertilizer
                startTime = curr_time;
                fertilizing = true;
                turn_on_fertilizer_pump();
            } else if ((curr_time - lastWater > wateringTimeout) && waterTank) { // check if plant needs water
                startTime = curr_time;
                watering = true;
                turn_on_water_pump();
            }
        }
    } else { // check if end of watering
        if (fertilizing) {
            unsigned long curr_time = millis();
            if ((curr_time - startTime > amountOfWater) || !fertilizerTank) {
                turn_off_fertilizer_pump();
                fertilizing = false;
                lastWater = curr_time;
                lastFertilizer = curr_time;
            }
        } else if (watering) {
            unsigned long curr_time = millis();
            if ((curr_time - startTime > amountOfWater) || !waterTank) {
                turn_off_water_pump();
                watering = false;
                lastWater = curr_time;
            }
        }
    }

    strcpy(response1, "your plant ");
    if (luminosityIntegral > luminosityThreshold) {
        // signal to raise luminosity of plant
        strcpy(response2, "needs less ");
        strcpy(response3, "light      ");
    } else if (luminosityIntegral < -luminosityThreshold) {
        // signal to lower luminosity of plant
        strcpy(response2, "needs more ");
        strcpy(response3, "   light   ");
    } else if (temperatureIntegral > temperatureThreshold) {
        // signal to raise temperature of plant
        strcpy(response2, "needs less ");
        strcpy(response3, "   heat    ");
    } else if (temperatureIntegral < -temperatureThreshold) {
        strcpy(response2, "needs more ");
        strcpy(response3, "   heat    ");
    } else if (humidityIntegral > humidityThreshold) {
        strcpy(response2, "needs less ");
        strcpy(response3, " humidity  ");
    } else if (humidityIntegral < -humidityThreshold) {
        strcpy(response2, "needs more ");
        strcpy(response3, " humidity  ");
    } else {
        strcpy(response2, "is healthy ");
        strcpy(response3, "           ");
    }

    ui.SendStatus(response1, response2, response3);

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
                ui.ScrollBackward();
            }
        }
        else
        {
            encoderCount--;
            if(encoderCount %4 ==0){
                ui.ScrollForward();
            }
        }
        #ifdef DEBUG
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
                stateChange = true;
                // state = ui.GetState(); // update state based on UI
                #ifdef DEBUG
                Serial.println("Button Pressed");
                #endif
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

#ifdef DEBUG
    Serial.printf("Luminosity: %d \n", luminosity);
    Serial.printf("Humidity: %d \n", humidity);
    Serial.printf("Temperature: %d \n", temperature);
#endif
}


void RetrievePastState()
{
    readBoolFromEEPROM(0, configured);
    readIntFromEEPROM(4, state);
    if (state < 0 || state > 4) {
        state = 0;
    }
    if (configured) {
        readIntFromEEPROM(8, plantType);
        ui.SetPlantType(plantType);
    }
}
