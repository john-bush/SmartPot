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
  xpos = x - radius;
  ypos = y - radius;
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

/// @brief Function to be used with parameter or non const char strings
              // Plots text p on screen centered at x,y
/// @param str String to plot
/// @param centerX horizontal center of the text
/// @param centerY vertical center of the text
/// @param scale_ Scale of text (scale 1 means each character is 6x8 pixels)
/// @param numChar number of characters to plot
void TFTDisplay::PlotTextCentered(const char* str, int centerX, int centerY, int scale_, int numChar)
{
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

/// @brief Plots text p on screen with bottom left corner at x,y
/// @param p const char string to plot
/// @param x Plots text with bottom left corner at x,y
/// @param y Plots text with bottom left corner at x,y
/// @param scale_ Scale of text (scale 1 means each character is 6x8 pixels)
/// @param background whether to plot the background color
void TFTDisplay::PlotTextLeftAligned(PGM_P p, int x, int y, int scale_, bool background = true)
{
  xpos = x;
  ypos = y;
  PlotText(p, scale_, background);
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
