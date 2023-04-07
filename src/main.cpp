#include "main.h"

float p = 3.1415926;

TFTDisplay Display = TFTDisplay();

void setup(void)
{
    Display.InitDisplay();
    Display.ClearDisplay();
    Display.DisplayOn();
    Display.MoveTo(0, 0);

    delay(2000);
    LoadScreen();

    // Color(0, 0, 255);
    // FillCircle(30);
    // TestChart();
}

void loop()
{

}

void LoadScreen()
{
    Display.FillScreen(Display.Green);
    Display.SetTextColor(Display.Black);
    Display.SetBackColor(Display.Green);
    Display.DrawImage(plant_medium, 36, 58, Display.center, Display.ysize*0.7);
    Display.PlotTextCentered(PSTR("SmartPOT"), Display.xsize>>1, Display.ysize*0.4, 3, true);
    Display.PlotTextCentered(PSTR("smarter plant care"), Display.xsize>>1, Display.ysize*0.25, 2, true);
    Display.PlotTextCentered(PSTR("V1.0"), Display.xsize>>1, Display.ysize*0.1, 1, true);
}

// void testlines(uint16_t color) {
//   Display.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < Display.width(); x+=6) {
//     Display.drawLine(0, 0, x, Display.height()-1, color);
//     delay(0);
//   }
//   for (int16_t y=0; y < Display.height(); y+=6) {
//     Display.drawLine(0, 0, Display.width()-1, y, color);
//     delay(0);
//   }

//   Display.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < Display.width(); x+=6) {
//     Display.drawLine(Display.width()-1, 0, x, Display.height()-1, color);
//     delay(0);
//   }
//   for (int16_t y=0; y < Display.height(); y+=6) {
//     Display.drawLine(Display.width()-1, 0, 0, y, color);
//     delay(0);
//   }

//   Display.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < Display.width(); x+=6) {
//     Display.drawLine(0, Display.height()-1, x, 0, color);
//     delay(0);
//   }
//   for (int16_t y=0; y < Display.height(); y+=6) {
//     Display.drawLine(0, Display.height()-1, Display.width()-1, y, color);
//     delay(0);
//   }

//   Display.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < Display.width(); x+=6) {
//     Display.drawLine(Display.width()-1, Display.height()-1, x, 0, color);
//     delay(0);
//   }
//   for (int16_t y=0; y < Display.height(); y+=6) {
//     Display.drawLine(Display.width()-1, Display.height()-1, 0, y, color);
//     delay(0);
//   }
// }

// void testdrawtext(char *text, uint16_t color) {
//   Display.setCursor(0, 0);
//   Display.setTextColor(color);
//   Display.setTextWrap(true);
//   Display.print(text);
// }

// void testfastlines(uint16_t color1, uint16_t color2) {
//   Display.fillScreen(ST77XX_BLACK);
//   for (int16_t y=0; y < Display.height(); y+=5) {
//     Display.drawFastHLine(0, y, Display.width(), color1);
//   }
//   for (int16_t x=0; x < Display.width(); x+=5) {
//     Display.drawFastVLine(x, 0, Display.height(), color2);
//   }
// }

// void testdrawrects(uint16_t color) {
//   Display.fillScreen(ST77XX_BLACK);
//   for (int16_t x=0; x < Display.width(); x+=6) {
//     Display.drawRect(Display.width()/2 -x/2, Display.height()/2 -x/2 , x, x, color);
//   }
// }

// void testfillrects(uint16_t color1, uint16_t color2) {
//   Display.fillScreen(ST77XX_BLACK);
//   for (int16_t x=Display.width()-1; x > 6; x-=6) {
//     Display.fillRect(Display.width()/2 -x/2, Display.height()/2 -x/2 , x, x, color1);
//     Display.drawRect(Display.width()/2 -x/2, Display.height()/2 -x/2 , x, x, color2);
//   }
// }

// void testfillcircles(uint8_t radius, uint16_t color) {
//   for (int16_t x=radius; x < Display.width(); x+=radius*2) {
//     for (int16_t y=radius; y < Display.height(); y+=radius*2) {
//       Display.fillCircle(x, y, radius, color);
//     }
//   }
// }

// void testdrawcircles(uint8_t radius, uint16_t color) {
//   for (int16_t x=0; x < Display.width()+radius; x+=radius*2) {
//     for (int16_t y=0; y < Display.height()+radius; y+=radius*2) {
//       Display.drawCircle(x, y, radius, color);
//     }
//   }
// }

// void testtriangles() {
//   Display.fillScreen(ST77XX_BLACK);
//   uint16_t color = 0xF800;
//   int t;
//   int w = Display.width()/2;
//   int x = Display.height()-1;
//   int y = 0;
//   int z = Display.width();
//   for(t = 0 ; t <= 15; t++) {
//     Display.drawTriangle(w, y, y, x, z, x, color);
//     x-=4;
//     y+=4;
//     z-=4;
//     color+=100;
//   }
// }

// void testroundrects() {
//   Display.fillScreen(ST77XX_BLACK);
//   uint16_t color = 100;
//   int i;
//   int t;
//   for(t = 0 ; t <= 4; t+=1) {
//     int x = 0;
//     int y = 0;
//     int w = Display.width()-2;
//     int h = Display.height()-2;
//     for(i = 0 ; i <= 16; i+=1) {
//       Display.drawRoundRect(x, y, w, h, 5, color);
//       x+=2;
//       y+=3;
//       w-=4;
//       h-=6;
//       color+=1100;
//     }
//     color+=100;
//   }
// }

// void tftPrintTest() {
//   Display.setTextWrap(false);
//   Display.fillScreen(ST77XX_BLACK);
//   Display.setCursor(0, 30);
//   Display.setTextColor(ST77XX_RED);
//   Display.setTextSize(1);
//   Display.println("Hello World!");
//   Display.setTextColor(ST77XX_YELLOW);
//   Display.setTextSize(2);
//   Display.println("Hello World!");
//   Display.setTextColor(ST77XX_GREEN);
//   Display.setTextSize(3);
//   Display.println("Hello World!");
//   Display.setTextColor(ST77XX_BLUE);
//   Display.setTextSize(4);
//   Display.print(1234.567);
//   delay(1500);
//   Display.setCursor(0, 0);
//   Display.fillScreen(ST77XX_BLACK);
//   Display.setTextColor(ST77XX_WHITE);
//   Display.setTextSize(0);
//   Display.println("Hello World!");
//   Display.setTextSize(1);
//   Display.setTextColor(ST77XX_GREEN);
//   Display.print(p, 6);
//   Display.println(" Want pi?");
//   Display.println(" ");
//   Display.print(8675309, HEX); // print 8,675,309 out in HEX!
//   Display.println(" Print HEX!");
//   Display.println(" ");
//   Display.setTextColor(ST77XX_WHITE);
//   Display.println("Sketch has been");
//   Display.println("running for: ");
//   Display.setTextColor(ST77XX_MAGENTA);
//   Display.print(millis() / 1000);
//   Display.setTextColor(ST77XX_WHITE);
//   Display.print(" seconds.");
// }

// void mediabuttons() {
//   // play
//   Display.fillScreen(ST77XX_BLACK);
//   Display.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
//   Display.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
//   delay(500);
//   // pause
//   Display.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
//   Display.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
//   Display.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
//   delay(500);
//   // play color
//   Display.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
//   delay(50);
//   // pause color
//   Display.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
//   Display.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
//   // play color
//   Display.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
// }
