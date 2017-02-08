#include "ir.hpp"
#include "gpio.hpp"

IR::IR(int channelGPIO) : channelGPIO(channelGPIO) {
	//make sure sensor line is input
	setDirectionGPIO(channelGPIO,1);
}

int IR::getData(void) {
	return readGPIO(channelGPIO);
}