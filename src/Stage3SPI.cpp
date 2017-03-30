#include "gpio.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "timing.hpp"
#include "SPIDevice.h" //include definitions

/*****************************************************
* Stage3SPI.cpp - An IEEE SECon 2017 program file    *
* Author(s):										 *
* Intent: Drive the stepper motor for Stage 3 using  *
* the SPI interface.                                 *
*****************************************************/

void Stage3SPI(int c1){
	cout << "Stepper Driver SPI Test" << endl;
	SPIDevice *stepper = new SPIDevice(0,0); //use bus 0, device id 0
	busDevice->setSpeed(200); //set speed to 200Hz (not sure if this works correctly yet)
	busDevice->setMode(SPIDevice::MODE0); //low idle, capture on rising clock edge
	int i = 0;

	unsigned char Pulse4[0]; //array used to write 0xAA

	Pulse4[0] = 0b10101010; //0xAA
	for (i = 0; i < 50){
		stepper->write(Pulse4, 1);
	}
	
	return 0;
}