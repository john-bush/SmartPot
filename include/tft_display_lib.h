#include <Arduino.h>

#if defined(MEGATINYCORE)
// ATtiny402/412 PORTA positions. Change these for the chip you're using
int const dc = 7;
int const mosi = 1;
int const sck = 3;
int const cs = 6;

// ATtiny 0-, 1-, and 2-series port manipulations - assumes all pins in same port
#define PORT_TOGGLE(x)  PORTA.OUTTGL = (x)
#define PORT_LOW(x)     PORTA.OUTCLR = (x)
#define PORT_HIGH(x)    PORTA.OUTSET = (x)
#define PORT_OUTPUT(x)  PORTA.DIRSET = (x)

#else
// ATtiny45/85 PORTB positions. Change these for the chip you're using
int const dc = 1;
int const mosi = 3;
int const sck = 5;
int const cs = 2;

// Classic ATtiny port manipulations - assumes all pins in same port
#define PORT_TOGGLE(x)  PINB = (x)
#define PORT_LOW(x)     PORTB = PORTB & ~((x));
#define PORT_HIGH(x)    PORTB = PORTB | ((x))
#define PORT_OUTPUT(x)  DDRB = (x)

#endif

void Data (uint8_t d);
void Command (uint8_t c);
void Command2 (uint8_t c, uint16_t d1, uint16_t d2);
void InitDisplay ();
void DisplayOn ();
void ClearDisplay ();
unsigned int Colour (int r, int g, int b);
void MoveTo (int x, int y);
void PlotPoint (int x, int y);
void DrawTo (int x, int y);
void FillRect (int w, int h);
void DrawRect (int w, int h);
void FillCircle (int radius);
void DrawCircle (int radius);
void PlotChar (char c);
void PlotText(PGM_P p);
void PlotInt(int n);
void TestChart ();
void BarChart ();
void Waterfall ();

