#include "ir.hpp"
#include "gpio.hpp"
#include "robotDefinitions.hpp"

IR::IR(int channelGPIO) : channelGPIO(channelGPIO) {
	//make sure sensor line is input
	setDirectionGPIO(channelGPIO,GPIO_INPUT);
}

int IR::getData(void) {
	return readGPIO(channelGPIO);
}