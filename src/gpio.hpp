#ifndef GPIO_HPP
#define GPIO_HPP

#define GPIO_FILE_PREFIX "/sys/class/gpio/gpio"
#define GPIO_OUTPUT 0
#define GPIO_INPUT 1

void writeGPIO(int,int);
int readGPIO(int);
void setDirectionGPIO(int,int);

#endif