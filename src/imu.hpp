#ifndef IMU_HPP
#define IMU_HPP

#include "i2cbus.hpp"

class IMU {
	//members
	private:
		static const int imu_address = 0x6a;
		I2Cbus *sensor_bus;
	
	//member functions
	public:
		IMU(I2Cbus*);
		float getTemp(void);
		float getGyroX(void);
		float getGyroY(void);
		float getGyroZ(void);
		float getAcclX(void);
		float getAcclY(void);
		float getAcclZ(void);
		float getAcclMag(void);
		float *getDataAll(void);
		void printData(void);
	private:
		static short combineRegisters(unsigned char,unsigned char);
};

#endif
