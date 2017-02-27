#include "display.hpp"
#include "i2cbus.hpp"
#include <math.h>
#include "glcdfont.hpp"
#include <string>
#include <string.h>
#include <iostream>

using namespace std;

Display::Display(I2Cbus* i2c_bus) : displayBuf_(0), dataBuf_(0), tmpBuf_(0), dispi2c_(0) {
  dispi2c_ = i2c_bus;
  dispi2c_->setDevice(DISP_ADDRESS);
  init();
}

Display::~Display() {
  if(displayBuf_) {
    delete [] displayBuf_;
    displayBuf_ = 0;
  }
}

void Display::clearBuffer() {
  if(displayBuf_)
    memset(displayBuf_, 0x00, (WIDTH * HEIGHT));
}

void Display::clearDisplay() {
  clearBuffer();
  writeDisplay();
}

bool Display::writeDisplay() {
  bool retstatus;
  unsigned short uiIndex = 0x00;
  unsigned char uchTemp[8];
  unsigned char tmp[3] = {0x21, 0x00, 0x7F};
  retstatus = dispi2c_->writebytesRegister(0x00,tmp, 4); // Set column addr, end 127
  tmp[0] = 0x22; tmp[2] = 0x07;
  retstatus = dispi2c_->writebytesRegister(0x00,tmp, 4);
  while(uiIndex < (WIDTH * HEIGHT)) {
    for(int i = 0; i < 8; i++)
      uchTemp[i] = displayBuf_[uiIndex+i];
    retstatus = dispi2c_->writebytesRegister(0x40,uchTemp,9); 
    uiIndex += 8;
  }
  return retstatus;
}

bool Display::writeCenter(string txt, unsigned char row) {
  if((txt.length()*6)<127)
    return writeText(txt, row, (128 - ((txt.length()*6)-1))/2);
  else
    return writeText(txt,row,0);
}

bool Display::writeText(string txt, unsigned char row, unsigned char col) {
  unsigned char uchI, uchJ;
  unsigned short uiIndex;

  uiIndex = (row * WIDTH) + col;

  if((row >= HEIGHT) || (col >= WIDTH)) 
    return false;

  for(uchI = 0; uchI < txt.length(); uchI++) {
    for(uchJ = 0; uchJ < 5; uchJ++) {
      displayBuf_[uiIndex] = font[(txt[uchI]*5)+uchJ];
      uiIndex++;
    }
    displayBuf_[uiIndex] = 0x00;
    uiIndex++;
  }
  return true;
}

bool Display::init() {
  bool retval = false;

  displayBuf_ = new unsigned char[WIDTH * HEIGHT];
  memset(displayBuf_, 0, (WIDTH*HEIGHT));
  if(!displayBuf_) {
    cout << "could not create displayBuf buffer" << endl;
    return retval;
  }
  dataBuf_ = new unsigned char [TILE_SIZE];
  memset(dataBuf_, 0, TILE_SIZE);
  if(!dataBuf_) {
    cout << "could not create dataBuf buffer" << endl;
  }

  tmpBuf_ = new unsigned char[TILE_SIZE];
  memset(tmpBuf_, 0, TILE_SIZE);

  if(!tmpBuf_) {
    cout<< "could not create tmpBuf_ buffer" << endl;
  }
  unsigned char tmpdata[3] = {0x00}; tmpdata[0] = 0xAE;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // Display Off
  tmpdata[0] = 0xD5; tmpdata[1] = 0x80;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 3); // Set Display CLock Div
  tmpdata[0] = 0xA8; tmpdata[1] = 0x3F;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 3); // Set Multiplex
  tmpdata[0] = 0xD3; tmpdata[1] = 0x00;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 3); // Set Display Offset
  tmpdata[0] = 0x40;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // Set Start Line 0
  tmpdata[0] = 0x8D; tmpdata[1] = 0x14;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 3); // Set Charge Pump
  tmpdata[0] = 0x20; tmpdata[1] = 0x00;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 3); // Memory Mode
  tmpdata[0] = 0xA1; 
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // 0xA1 = 127 = SEG0
  tmpdata[0] = 0xC8;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // 0xC8 , scan from Com[n-1] to Com 0
  tmpdata[0] = 0xDA; tmpdata[1] = 0x12;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 3); // Set comm pins
  tmpdata[0] = 0x81; tmpdata[1] = 0xCF;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 3); // Set contrast
  tmpdata[0] = 0xD9; tmpdata[1] = 0xF1; 
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 3); // Set precharge
  tmpdata[0] = 0xDB; tmpdata[1] = 0x40;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 3); // Set VCOM select
  tmpdata[0] = 0xA4;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // Resume RAM content display
  tmpdata[0] = 0xA6;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // Normal display not inverted
  tmpdata[0] = 0x00;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // low col = 0
  tmpdata[0] = 0x10;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // high col = 0
  tmpdata[0] = 0x40;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // line # 0
  tmpdata[0] = 0xAF;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // display on
  tmpdata[0] = 0x21; tmpdata[1] = 0x00; tmpdata[2] = 0x7F;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 4); // Set column address; start 0, end 127
  tmpdata[0] = 0x22; tmpdata[2] = 0x07;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 4); // Set row address; start 0, end 7
  tmpdata[0] = 0xAF;
  retval |= dispi2c_->writebytesRegister(0x00,tmpdata, 2); // display on
  return retval;

}

bool Display::writeImage (unsigned char* image) { 
  for (int lcv=0; lcv<(WIDTH * HEIGHT); lcv++) {
    getTileFromBitmap(lcv);
    // rotate the 8x8 tile
    rotateTile();
    for(int i = 0; i < TILE_SIZE; i++) {
      tmpBuf_[i] = reverseByte(tmpBuf_[i]);
    }
    setTileInBuffer(lcv);
  }
  return false;
}

unsigned char Display::reverseByte(unsigned char b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

void Display::getTileFromBitmap(int index) {
  int step = 0;
  for(int i = 0; i < TILE_SIZE; i++, step+= STEP_SIZE) {
    dataBuf_[i] = displayBuf_[index+step];
  }
}

void Display::setTileInBuffer(int index) {
  int step = 0;
  for(int i = 0; i < TILE_SIZE; i++, step+= STEP_SIZE) {
    displayBuf_[index+step] |=tmpBuf_[i];
  }
}

void Display::rotateTile()
{
   // process bits for a bytes in data
   for (int i=0; i<TILE_SIZE; i++)
   {
      // process data bytes
      for (int j=0; j<TILE_SIZE; j++)
      {
         unsigned char temp = dataBuf_[j] & (unsigned char)pow(2, 7-i);
         int shift = 7-i-j;
         unsigned char shifted = 0x0;
         if (shift < 0)
         {
            shift *= -1;
            shifted = temp << shift;
         }
         else
         {
            shifted = temp >> shift;
         }
         tmpBuf_[i] |= shifted;
      }
      tmpBuf_[i] = reverseByte(tmpBuf_[i]);
   }
}





