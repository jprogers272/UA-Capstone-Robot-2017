//Author(s): John Rogers

#ifndef COMPASS_HPP
#define COMPASS_HPP

#include "i2cbus.hpp"

class Compass {
	//members
	private:
		static const int address = 0x60;
		I2Cbus *sensor_bus;
	
	//member functions
	public:
		Compass(I2Cbus*);
		float getAngleF(void);
		int getAngleI(void);
		int getAngleRaw(void);
		
	private:
		static short combineRegisters(unsigned char,unsigned char);
};

#endif