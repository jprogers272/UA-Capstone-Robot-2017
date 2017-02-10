#include "compass.hpp"
#include "i2cbus.hpp"

Compass::Compass(I2Cbus *i2c_bus) : sensor_bus(i2c_bus) {
	//no constructor statements are needed
}

//returns a float in the range [0.0, 359.9]
float Compass::getAngleF(void) {
	int angle_raw = this->getAngleRaw();
	float angle = angle_raw / 10.0;
	
	return angle;
}

//returns an int in the range [0, 360]
int Compass::getAngleI(void) {
	int angle_raw = this->getAngleRaw();
	int angle = angle_raw / 10;
	//do basic rounding to account for integer division truncation
	if ((angle_raw % 10) >= 5) {
		angle++;
	}
	
	return angle;
}

//returns an int in the range [0, 3599], as returned by the sensor itself
int Compass::getAngleRaw(void) {
	sensor_bus->setDevice(address);
	
	unsigned char *readBuffer;
	readBuffer = sensor_bus->readRegisters(0x02,0x03);
	
	int angle_raw = combineRegisters(readBuffer[0],readBuffer[1]);
	
	delete(readBuffer);
	
	return angle_raw;
}

short Compass::combineRegisters(unsigned char msB, unsigned char lsB) {
	return ((short)msB<<8) | lsB;
}