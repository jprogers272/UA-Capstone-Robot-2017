#include "i2cbus.hpp"
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

using namespace std;

I2Cbus::I2Cbus(int bus) {
	this->bus = bus;
	device_address = -1;
	openFile();
}

I2Cbus::~I2Cbus(void) {
	close(file_i2c);
}

int I2Cbus::openFile(void) {
	ostringstream file_path;
	file_path << "/dev/i2c-" << bus;
	if ((file_i2c = open(file_path.str().c_str(), O_RDWR)) < 0) {
		perror("failed to open bus file\n");
		return 1;
	}
	return 0;
}
	
int I2Cbus::setDevice(int device_address) {
	if (device_address == this->device_address) {
		return 0;
	}
	if (ioctl(file_i2c, I2C_SLAVE, device_address) < 0) {
		perror("Failed to get device\n");
		return 1;
	}
	this->device_address = device_address;
	return 0;
}

unsigned char I2Cbus::readRegisterSingle(unsigned int registerAddress) {
	setRegisterAddress(registerAddress);
	unsigned char data[1];
	if (read(file_i2c, data, 1) != 1) {
		perror("Failed to read in data");
		return 1;
	}
	return data[0];
}

unsigned char* I2Cbus::readRegisters(unsigned int fromAddr, unsigned int toAddr) {
	setRegisterAddress(fromAddr);
	int length = toAddr - fromAddr + 1;
	unsigned char *data = new unsigned char[length];
	if (read(file_i2c, data, length) != length) {
		perror("Failed to read in data\n");
		return NULL;
	}
	return data;
}

int I2Cbus::setRegisterAddress(unsigned int registerAddress) {
	char writeBuffer[1];
	writeBuffer[0] = registerAddress;
	if (write(file_i2c, writeBuffer, 1) != 1) {
		perror("Failed to set register address\n");
		return 1;
	}
	return 0;
}

int I2Cbus::writeRegister(unsigned int registerAddress, unsigned char value) {
	unsigned int writeBuffer[2];
	writeBuffer[0] = registerAddress;
	writeBuffer[1] = value;
	if (write(file_i2c, writeBuffer, 1) != 1) {
		perror("Failed to write to device");
		return 1;
	}
	return 0;
}

/*
short combineRegisters(unsigned char msB, unsigned char lsB) {
	return ((short)msB<<8) | lsB;
}
*/