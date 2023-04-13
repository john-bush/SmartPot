#include "main.h"
#include "i2c.h"
#include "tsl2591.h"
#include "dht20.h"

float p = 3.1415926;

Interface ui = Interface();
DHT20 dht = DHT20();

void setup(void)
{
    InitEncoder();
    ui.Init();
    //ui.LoadingScreen();

    i2c_init(BDIV);

    ui.ClearDisplay();

    // Color(0, 0, 255);
    // FillCircle(30);
    // TestChart();
}


void loop()
{
    // ui.DrawTankScreen();

    // delay(2000);
    // ui.UpdateTank(0, 1);

    // delay(2000);
    // ui.UpdateTank(1, 1);

    // delay(2000);
    // ui.UpdateTank(0, 0);


    // uint32_t lum = tsl_enable();  //rd_tsl_luminosity();
    // ui.MoveTo(160,40);
    // ui.SetTextColor(0xFFFF);
    // ui.PlotInt(lum);
    // delay(1000);

    uint32_t lum = rd_tsl_luminosity();
    ui.MoveTo(80, 80);
    ui.SetTextColor(0xFFFF);
    
    ui.PlotText(PSTR("Luminosity: "));
    ui.PlotInt(lum);

    uint8_t res = dht.full_measurement();
    ui.MoveTo(80, 60);
    ui.PlotText(PSTR("Temperature: "));
    ui.PlotInt( /*res);*/dht.get_temperature());
    ui.MoveTo(80, 40);
    ui.PlotText(PSTR("Humidity: "));
    ui.PlotInt(dht.get_humidity());



    delay(1000);
    ui.ClearDisplay();

    // lum = tsl_disable();
    // ui.MoveTo(160, 0);
    // ui.PlotInt(lum);

    // delay(1000);

    //ReadSoil();
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

void ReadSoil(){
  int soil_read = analogRead(PC0);
  ui.MoveTo(160, 160);
  delay(1000);
  ui.PlotInt(soil_read);
}
