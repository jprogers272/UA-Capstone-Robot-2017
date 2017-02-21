#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "DCmotor.hpp"
#include "sensors.hpp"

enum State {
	zero_gyro,
	pre_stage1,
	stage1,
	post_stage1,
	pre_stage2,
	average_compass,
	stage2,
	post_stage2,
	pre_stage3,
	stage3,
	post_stage3,
	pre_stage4,
	stage4
}

class Robot {
	//members
	private:
		SensorData *sensorData;
		DCmotor wheel_1;
		DCmotor wheel_2;
		DCmotor wheel_3;
		DCmotor wheel_4;
		DCmotor slapper;
		State currentState;
		State nextState;
		float *drive_voltages;
		float slapper_voltage;
	
	//member functions
	public:
		Robot(void);
		int robotLogic(void);
		void setOutputs(void);
	
	private:
		void zeroVoltages(void);
}

#endif