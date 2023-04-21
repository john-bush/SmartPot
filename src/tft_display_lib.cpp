#include "tft_display_lib.h"

// Send a byte to the display

void TFTDisplay::Data(uint8_t d)
{
  for (uint8_t bit = 0x80; bit; bit >>= 1)
  {
    PORT_TOGGLE(1 << sck);
    if (d & bit)
      PORT_HIGH(1 << mosi);
    else
      PORT_LOW(1 << mosi);
    PORT_TOGGLE(1 << sck);
  }
}

// Send a command to the display
void TFTDisplay::Command(uint8_t c)
{
  PORT_TOGGLE(1 << dc);
  Data(c);
  PORT_TOGGLE(1 << dc);
}

// Send a command followed by two data words
void TFTDisplay::Command2(uint8_t c, uint16_t d1, uint16_t d2)
{
  PORT_TOGGLE(1 << dc);
  Data(c);
  PORT_TOGGLE(1 << dc);
  Data(d1 >> 8);
  Data(d1);
  Data(d2 >> 8);
  Data(d2);
}

void TFTDisplay::InitDisplay()
{
  PORT_OUTPUT(1 << dc | 1 << cs | 1 << mosi | 1 << sck); // All outputs
  PORT_HIGH(1 << dc | 1 << cs | 1 << sck);               // Outputs high
  PORT_TOGGLE(1 << cs);
  Command(0x01); // Software reset
  delay(250);    // delay 250 ms
  Command(0x36);
  Data(rotate << 5 | bgr << 3); // Set orientation and rgb/bgr
  Command(0x3A);
  Data(0x55);             // Set color mode - 16-bit color
  Command(0x20 + invert); // Invert
  Command(0x11);          // Out of sleep mode
  delay(150);
  PORT_TOGGLE(1 << cs);
}

void TFTDisplay::DisplayOn()
{
  PORT_TOGGLE(1 << cs);
  Command(0x29); // Display on
  delay(150);
  PORT_TOGGLE(1 << cs);
}

void TFTDisplay::ClearDisplay()
{
  PORT_TOGGLE(1 << cs);
  Command2(CASET, yoff, yoff + ysize - 1);
  Command2(RASET, xoff, xoff + xsize - 1);
  Command(0x3A);
  Data(0x03);     // 12-bit color
  Command(RAMWR); // Leaves mosi low
  for (int i = 0; i < xsize * 4; i++)
  {
    for (int j = 0; j < ysize * 3; j++)
    {
      PORT_TOGGLE(1 << sck);
      PORT_TOGGLE(1 << sck);
    }
  }
  Command(0x3A);
  Data(0x05); // Back to 16-bit color
  PORT_TOGGLE(1 << cs);
}

void TFTDisplay::FillScreen(int color)
{
  MoveTo(0, 0);
  int store_color = fore;
  fore = color;
  FillRect(xsize, ysize);
  fore = store_color;
  back = color;
}

unsigned int TFTDisplay::Color(int r, int g, int b)
{
  return (r & 0xf8) << 8 | (g & 0xfc) << 3 | b >> 3;
}

void TFTDisplay::SetForeColor(int r, int g, int b)
{
  fore = ((r & 0xf8) << 8 | (g & 0xfc) << 3 | b >> 3);
}

void TFTDisplay::SetTextColor(int r, int g, int b)
{
  textColor = ((r & 0xf8) << 8 | (g & 0xfc) << 3 | b >> 3);
}

void TFTDisplay::SetBackColor(int r, int g, int b)
{
  back = ((r & 0xf8) << 8 | (g & 0xfc) << 3 | b >> 3);
}

void TFTDisplay::SetForeColor(int color) { fore = color; }

void TFTDisplay::SetTextColor(int color) { textColor = color; }

void TFTDisplay::SetBackColor(int color) { back = color; }

// Move current plot position to x,y
void TFTDisplay::MoveTo(int x, int y)
{
  xpos = x;
  ypos = y;
}

// Plot point at x,y
void TFTDisplay::PlotPoint(int x, int y)
{
  PORT_TOGGLE(1 << cs);
  Command2(CASET, yoff + y, yoff + y);
  Command2(RASET, xoff + x, xoff + x);
  Command(RAMWR);
  Data(fore >> 8);
  Data(fore & 0xff);
  PORT_TOGGLE(1 << cs);
}

void TFTDisplay::DrawImage(const uint16_t *image, int w, int h, int centerX, int centerY, bool mirror, int scale)
{
  int imageOriginX = centerX - (scale * w) / 2;
  int imageOriginY = centerY + (scale * h) / 2;
  int fore_copy = fore;
  if (mirror){
    Serial.println("Mirrored Image");
  }
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      //fore = pgm_read_word(&image[y * w + x]); // this works
      if (mirror)
      {
        fore = pgm_read_word(&image[y * w + (w - x - 1)]); // this needs to be tested
      } else {
        fore = pgm_read_word(&image[y * w + x]);
      }
      for (int i = 0; i < scale; i++)
      {
        for (int j = 0; j < scale; j++)
        {
          PlotPoint(imageOriginX + (scale * x) + i, imageOriginY - (scale * y) - j);  // needs to be tested
        }
      }
      // PlotPoint(imageOriginX + x, imageOriginY - y); // wordks
    }
  }
  fore = fore_copy; // revert to old fore color
}

// Draw a line to x,y
void TFTDisplay::DrawTo(int x, int y)
{
  int sx, sy, e2, err;
  int dx = abs(x - xpos);
  int dy = abs(y - ypos);
  if (xpos < x)
    sx = 1;
  else
    sx = -1;
  if (ypos < y)
    sy = 1;
  else
    sy = -1;
  err = dx - dy;
  for (;;)
  {
    PlotPoint(xpos, ypos);
    if (xpos == x && ypos == y)
      return;
    e2 = err << 1;
    if (e2 > -dy)
    {
      err = err - dy;
      xpos = xpos + sx;
    }
    if (e2 < dx)
    {
      err = err + dx;
      ypos = ypos + sy;
    }
  }
}

// Draw a line to x,y
void TFTDisplay::DrawTo(int x1, int y1, int x2, int y2)
{
  int sx, sy, e2, err;
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  if (x1 < x2)
    sx = 1;
  else
    sx = -1;
  if (y1 < y2)
    sy = 1;
  else
    sy = -1;
  err = dx - dy;
  for (;;)
  {
    PlotPoint(x1, y1);
    if (x1 == x2 && y1 == y2)
      return;
    e2 = err << 1;
    if (e2 > -dy)
    {
      err = err - dy;
      x1 = x1 + sx;
    }
    if (e2 < dx)
    {
      err = err + dx;
      y1 = y1 + sy;
    }
  }
}

void TFTDisplay::FillRect(int w, int h)
{
  PORT_TOGGLE(1 << cs);
  Command2(CASET, ypos + yoff, ypos + yoff + h - 1);
  Command2(RASET, xpos + xoff, xpos + xoff + w - 1);
  Command(RAMWR);
  uint8_t hi = fore >> 8;
  uint8_t lo = fore & 0xff;
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      Data(hi);
      Data(lo);
    }
  }
  PORT_TOGGLE(1 << cs);
}

void TFTDisplay::FillRect(int w, int h, int x, int y)
{
  xpos = x;
  ypos = y;
  FillRect(w, h);
}

void TFTDisplay::FillCenteredRect(int w, int h, int centerX, int centerY) {
  xpos = centerX - w / 2;
  ypos = centerY - h / 2;
  FillRect(w, h);
}

void TFTDisplay::DrawRect(int w, int h)
{
  int x1 = xpos, y1 = ypos;
  FillRect(w - 1, 1);
  MoveTo(x1, y1 + 1);
  FillRect(1, h - 1);
  MoveTo(x1 + 1, y1 + h - 1);
  FillRect(w - 1, 1);
  MoveTo(x1 + w - 1, y1);
  FillRect(1, h - 1);
  xpos = x1;
  ypos = y1;
}

void TFTDisplay::DrawRect(int w, int h, int x, int y)
{
  xpos = x;
  ypos = y;
  DrawRect(w, h);
}

void TFTDisplay::DrawCenteredRect(int w, int h, int x, int y, int thickness)
{
  
  for (int i = 0; i < thickness; i++)
  {
    xpos = x - w / 2 - i;
    ypos = y - h / 2 - i;
    DrawRect(w + 2 * i, h + 2 * i);
  }
}

void TFTDisplay::FillCircle(int radius)
{
  int x1 = xpos, y1 = ypos, dx = 1, dy = 1;
  int x = radius - 1, y = 0;
  int err = dx - (radius << 1);
  while (x >= y)
  {
    MoveTo(x1 - x, y1 + y);
    FillRect(x << 1, 1);
    MoveTo(x1 - y, y1 + x);
    FillRect(y << 1, 1);
    MoveTo(x1 - y, y1 - x);
    FillRect(y << 1, 1);
    MoveTo(x1 - x, y1 - y);
    FillRect(x << 1, 1);
    if (err > 0)
    {
      x = x - 1;
      dx = dx + 2;
      err = err - (radius << 1) + dx;
    }
    else
    {
      y = y + 1;
      err = err + dy;
      dy = dy + 2;
    }
  }
  xpos = x1;
  ypos = y1;
}

void TFTDisplay::FillCircle(int radius, int x, int y)
{
  xpos = x;
  ypos = y;
  FillCircle(radius);
}

void TFTDisplay::DrawCircle(int radius)
{
  int x1 = xpos, y1 = ypos, dx = 1, dy = 1;
  int x = radius - 1, y = 0;
  int err = dx - (radius << 1);
  while (x >= y)
  {
    PlotPoint(x1 - x, y1 + y);
    PlotPoint(x1 + x, y1 + y);
    PlotPoint(x1 - y, y1 + x);
    PlotPoint(x1 + y, y1 + x);
    PlotPoint(x1 - y, y1 - x);
    PlotPoint(x1 + y, y1 - x);
    PlotPoint(x1 - x, y1 - y);
    PlotPoint(x1 + x, y1 - y);
    if (err > 0)
    {
      x = x - 1;
      dx = dx + 2;
      err = err - (radius << 1) + dx;
    }
    else
    {
      y = y + 1;
      err = err + dy;
      dy = dy + 2;
    }
  }
}

void TFTDisplay::DrawCircle(int radius, int x, int y)
{
  xpos = x;
  ypos = y;

  DrawCircle(radius);
}

// Plot an ASCII character with bottom left corner at x,y
void TFTDisplay::PlotChar(char c, bool background)
{
  int color;
  PORT_TOGGLE(1 << cs);
  Command2(CASET, yoff + ypos, yoff + ypos + 8 * scale - 1);
  Command2(RASET, xoff + xpos, xoff + xpos + 6 * scale - 1);
  Command(RAMWR);
  for (int xx = 0; xx < 6; xx++)
  {
    int bits = pgm_read_byte(&CharMap[c - 32][xx]);
    for (int xr = 0; xr < scale; xr++)
    {
      for (int yy = 0; yy < 8; yy++)
      {
        if (bits >> (7 - yy) & 1)
        {
          color = textColor;
        }
        else if (background)
        {
          color = back;
        }
        else
        {
          continue;
        }
        for (int yr = 0; yr < scale; yr++)
        {
          Data(color >> 8);
          Data(color & 0xFF);
        }
      }
    }
  }
  PORT_TOGGLE(1 << cs);
  xpos = xpos + 6 * scale; // Advance to next character position
}


void TFTDisplay::PlotChar(char c, int x, int y)
{
  xpos = x;
  ypos = y;
  PlotChar(c);
}

// Plot text starting at the current plot position
void TFTDisplay::PlotText(PGM_P p)
{
  while (1)
  {
    char c = pgm_read_byte(p++);
    if (c == 0)
      return;
    PlotChar(c);
  }
}

// Plot text starting at the current plot position
void TFTDisplay::PlotText(PGM_P p, int scale_, bool background)
{
  scale = scale_;
  while (1)
  {
    char c = pgm_read_byte(p++);
    if (c == 0)
      return;
    PlotChar(c, background);
  }
}

void TFTDisplay::PlotTextCentered(PGM_P p, int centerX, int centerY, int scale_, bool background)
{
  int len = 0;
  PGM_P q = p;
  while (1)
  {
    char c = pgm_read_byte(q++);
    if (c == 0)
      break;
    len = len + 6 * scale_;
  }
  xpos = centerX - (len >> 1);
  ypos = centerY - 4 * scale_;
  PlotText(p, scale_, background);
}

void TFTDisplay::PlotTextCentered(const char* str, int centerX, int centerY, int scale_, int color)
{
  int numChar = 12;
  int len = 0;
  scale = scale_;
  // Count the number of characters in the string
  len = numChar * 6 * scale_;

  xpos = centerX - (len >> 1);
  ypos = centerY - 4 * scale_;
  
  // Print each character on a separate line
  for (int i = 0; i < numChar; i++) {
    PlotChar(str[i], true);
  }
}

void TFTDisplay::PlotInt(int n)
{
  bool lead = false;
  for (uint32_t d = 10000000; d > 0; d = d / 10)
  {
    char j = (n / d) % 10;
    if (j != 0 || lead || d == 1)
    {
      PlotChar(j + '0');
      lead = true;
    }
  }
}

void TFTDisplay::TestChart()
{
  DrawRect(xsize, ysize);
  scale = 8;
  fore = Color(255, 0, 0);
  MoveTo((xsize - 40) / 2, (ysize - 64) / 2);
  PlotChar('F');
  scale = 1;
}

// Demos **********************************************

void TFTDisplay::BarChart()
{
  int x0 = 0, y0 = 0, w = xsize, h = ysize, x1 = 15, y1 = 11;
  MoveTo(x0 + (w - x1 - 90) / 2 + x1, y0 + h - 8);
  PlotText(PSTR("Sensor Readings"));
  // Horizontal axis
  int xinc = (w - x1) / 20;
  MoveTo(x0 + x1, y0 + y1);
  DrawTo(x0 + w - 1, y0 + y1);
  for (int i = 0; i <= 20; i = i + 4)
  {
    int mark = x1 + i * xinc;
    MoveTo(x0 + mark, y0 + y1);
    DrawTo(x0 + mark, y0 + y1 - 2);
    // Draw histogram
    if (i != 20)
    {
      int bar = xinc * 4 / 3;
      for (int b = 2; b >= 0; b--)
      {
        fore = Color(255, 127 * b, 0); // Red, Orange, Yellow
        MoveTo(x0 + mark + bar * b - b + 1, y0 + y1 + 1);
        FillRect(bar, 5 + random(h - y1 - 20));
      }
      fore = White;
    }
    if (i > 9)
      MoveTo(x0 + mark - 7, y0 + y1 - 11);
    else
      MoveTo(x0 + mark - 3, y0 + y1 - 11);
    PlotInt(i);
  }
  // Vertical axis
  int yinc = (h - y1) / 20;
  MoveTo(x0 + x1, y0 + y1);
  DrawTo(x0 + x1, y0 + h - 1);
  for (int i = 0; i <= 20; i = i + 5)
  {
    int mark = y1 + i * yinc;
    MoveTo(x0 + x1, y0 + mark);
    DrawTo(x0 + x1 - 2, y0 + mark);
    if (i > 9)
      MoveTo(x0 + x1 - 15, y0 + mark - 4);
    else
      MoveTo(x0 + x1 - 9, y0 + mark - 4);
    PlotInt(i);
  }
}

void TFTDisplay::Waterfall()
{
  int x0 = 0, y0 = 0, w = xsize, h = ysize, x1 = 15, y1 = 11;
  int factor = 5160 / h * 10;
  MoveTo(x0 + (w - x1 - 60) / 2 + x1, y0 + h - 8);
  PlotText(PSTR("Luminance"));
  // Horizontal axis
  int xinc = (w - x1 - 15) / 30;
  MoveTo(x0 + x1, y0 + y1);
  DrawTo(x0 + x1 + xinc * 20, y0 + y1);
  for (int i = 0; i <= 20; i = i + 5)
  {
    int mark = x1 + i * xinc;
    MoveTo(x0 + mark, y0 + y1);
    DrawTo(x0 + mark, y0 + y1 - 2);
    if (i > 9)
      MoveTo(x0 + mark - 7, y0 + y1 - 11);
    else
      MoveTo(x0 + mark - 3, y0 + y1 - 11);
    PlotInt(i);
  }
  // Vertical axis
  int yinc = (h - y1) / 20;
  MoveTo(x0 + x1, y0 + y1);
  DrawTo(x0 + x1, y0 + h - 1);
  for (int i = 0; i <= 20; i = i + 5)
  {
    int mark = y1 + i * yinc;
    MoveTo(x0 + x1, y0 + mark);
    DrawTo(x0 + x1 - 2, y0 + mark);
    if (i > 9)
      MoveTo(x0 + x1 - 15, y0 + mark - 4);
    else
      MoveTo(x0 + x1 - 9, y0 + mark - 4);
    PlotInt(i);
  }
  // Diagonal axis
  yinc = xinc / 2;
  // MoveTo(x0+x1, y0+y1); DrawTo(x0+x1+10*xinc, y0+y1+10*xinc);
  MoveTo(x0 + x1 + 20 * xinc, y0 + y1);
  DrawTo(x0 + x1 + 30 * xinc, y0 + y1 + 10 * xinc);
  for (int i = 0; i <= 20; i = i + 5)
  {
    MoveTo(x0 + x1 + 20 * xinc + i * xinc / 2, y0 + y1 + i * xinc / 2);
    DrawTo(x0 + x1 + 20 * xinc + i * xinc / 2 + 3, y0 + y1 + i * xinc / 2);
    MoveTo(x0 + x1 + 20 * xinc + i * xinc / 2 + 6, y0 + y1 + i * xinc / 2 - 4);
    PlotInt(i);
  }
  // Plot data
  for (int y = 20; y >= 0; y--)
  {
    for (int i = 0; i <= 20; i++)
    {
      int fn0 = 180 - (i - 10) * (i - 10) - (y - 10) * (y - 10);
      int fn1 = 180 - (i + 1 - 10) * (i + 1 - 10) - (y - 10) * (y - 10);
      fore = Color(255, 255, 0);
      MoveTo(x0 + x1 + y * yinc + i * xinc, y0 + y1 + y * yinc + fn0 * fn0 / factor);
      DrawTo(x0 + x1 + y * yinc + (i + 1) * xinc, y0 + y1 + y * yinc + fn1 * fn1 / factor);
      fore = White;
    }
  }
}


