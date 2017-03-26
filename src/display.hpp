#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "i2cbus.hpp"
#include <string>

#define WIDTH 128
#define HEIGHT 8
#define TILE_SIZE 8
#define STEP_SIZE 16

#define DISP_ADDRESS 0x3C



class Display {
  public:
    Display(I2Cbus*);
    ~Display();
    void clearBuffer();
    void clearDisplay();
    void clearAll();
    void clearDisplayAll();
    void writeDisplay();
    void writeCenter(std::string txt, unsigned char row);
    void writeText(std::string txt, unsigned char row, unsigned char col);
    void writeImage(unsigned char* img);
  
  private:
    void init ();
    unsigned char reverseByte (unsigned char b);
    void getTileFromBitmap (int index);
    void setTileInBuffer(int index);
    void rotateTile();

    unsigned char *dataBuf_;
    unsigned char *tmpBuf_;
    I2Cbus *dispi2c_;
};

#endif
