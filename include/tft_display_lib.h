#pragma once

#include <Arduino.h>
#include "graphics.h"

// Pin assignments for ATMega328P
int const dc = 1;
int const mosi = 3;
int const sck = 5;
int const cs = 2;

// Classic ATtiny port manipulations - assumes all pins in same port
#define PORT_TOGGLE(x)  PINB = (x)
#define PORT_LOW(x)     PORTB = PORTB & ~((x));
#define PORT_HIGH(x)    PORTB = PORTB | ((x))
#define PORT_OUTPUT(x)  DDRB = (x)


class TFTDisplay {
    private:
        void Data (uint8_t d);
        void Command (uint8_t c);
        void Command2 (uint8_t c, uint16_t d1, uint16_t d2);
        
        // TFT color display **********************************************
        int const CASET = 0x2A; // Define column address
        int const RASET = 0x2B; // Define row address
        int const RAMWR = 0x2C; // Write to display RAM

        // Current plot position and colors
        int xpos, ypos;
        int fore = White;
        int textColor = White;
        int back = Black;
        int scale = 1;     // Text scale
   
    public:
        int const White = 0xFFFF;
        int const Black = 0;
        int const Green = 0xefb8; // special plant green color

        void InitDisplay ();
        void DisplayOn ();
        void ClearDisplay ();
        void FillScreen(int color);
        unsigned int Color (int r, int g, int b);
        void SetForeColor (int r, int g, int b);
        void SetTextColor (int r, int g, int b);
        void SetBackColor (int r, int g, int b);
        void SetForeColor (int color);
        void SetTextColor (int color);
        void SetBackColor (int color);

        void MoveTo (int x, int y);
        void PlotPoint (int x, int y);

        /**
         * Functions that do not include a position argument will draw relative to the current position
        */

        // draws an image with width w and height h defined by color map *image centered at position x,y
        void DrawImage(const uint16_t *image, int w, int h, int centerX, int centerY, bool mirror, int scale = 1);
        // Draws line from current position to x,y
        void DrawTo (int x, int y);
        // Draws a line between two points
        void DrawTo (int x1, int y1, int x2, int y2);
        // Fills a Rectangle with current foreground color, with bottom left corner at current position
        void FillRect (int w, int h);
        void FillRect (int w, int h, int x, int y);
        void FillCenteredRect(int w, int h, int centerX, int centerY);
        void DrawRect (int w, int h);
        void DrawRect (int w, int h, int x, int y);
        void DrawCenteredRect(int w, int h, int x, int y, int thickness);
        void FillCircle (int radius);
        void FillCircle (int radius, int x, int y);
        void DrawCircle (int radius);
        void DrawCircle (int radius, int x, int y);
        void PlotChar (char c, bool background = true);
        void PlotChar(char c, int x, int y);
        void PlotText(PGM_P p);
        void PlotText(PGM_P p, int scale_, bool background = true);

        /**
         * Writes Text block with center at position x, y
        */
        void PlotTextCentered(PGM_P p, int x, int y, int scale_, bool background = true);
        void PlotTextCentered(const char* str, int centerX, int centerY, int scale_, int color);
        void PlotTextLeftAligned(PGM_P p, int x, int y, int scale_, bool background = true);
        void PlotInt(int n);

        // Adafruit 1.47" 320x172 rounded rectangle display
        int const xsize = 320, ysize = 172, xoff = 0, yoff = 34, invert = 1, rotate = 0, bgr = 0;
        const int center = xsize / 2;
};


