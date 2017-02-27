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
    bool writeDisplay();
    bool writeCenter(std::string txt, unsigned char row);
    bool writeText(std::string txt, unsigned char row, unsigned char col);
    bool writeImage(unsigned char* img);
  
  private:
    bool init ();
    unsigned char reverseByte (unsigned char b);
    void getTileFromBitmap (int index);
    void setTileInBuffer(int index);
    void rotateTile();

    unsigned char* displayBuf_;
    unsigned char* dataBuf_;
    unsigned char* tmpBuf_;
    I2Cbus *dispi2c_;
};

#endif
