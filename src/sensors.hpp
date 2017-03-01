//Author(s): John Rogers

#ifndef SENSORS_HPP
#define SENSORS_HPP

#include "compass.hpp"
#include "imu.hpp"
#include "ir.hpp"

struct SensorData {
	int ir1_1_state;
	int ir1_2_state;
	int ir1_3_state;
	int ir1_4_state;
	int ir2_1_state;
	int ir2_2_state;
	int ir2_3_state;
	int ir2_4_state;
	int ir4_1_state;
	int ir4_2_state;
	float gyroZ;
	float gyroY;
	float acclY;
	float compass_angle;
	float battery_voltage;
};

class Sensors {
	//members
	private:
		IR ir1_1;
		IR ir1_2;
		IR ir1_3;
		IR ir1_4;
		IR ir2_1;
		IR ir2_2;
		IR ir2_3;
		IR ir2_4;
		IR ir4_1;
		IR ir4_2;
		IMU imu;
		Compass compass;
	
	//member functions
	public:
		Sensors(I2Cbus*);
		void getAllSensors(SensorData*);
};

#endif
