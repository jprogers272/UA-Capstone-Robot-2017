//Author(s): John Rogers

#ifndef GPIO_HPP
#define GPIO_HPP

#define GPIO_FILE_PREFIX "/sys/class/gpio/gpio"

void writeGPIO(int,int);
int readGPIO(int);
void setDirectionGPIO(int,int);

#endif