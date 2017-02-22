#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "DCmotor.hpp"
#include "sensors.hpp"

enum State {
	start,
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
	stage4,
	finish
};

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
		
		void start_logic(void);
		void zero_gyro_logic(void);
		void pre_stage1_logic(void);
		void stage1_logic(void);
		void post_stage1_logic(void);
		void pre_stage2_logic(void);
		void average_compass_logic(void);
		void stage2_logic(void);
		void post_stage2_logic(void);
		void pre_stage3_logic(void);
		void stage3_logic(void);
		void post_stage3_logic(void);
		void pre_stage4_logic(void); 
		void stage4_logic(void);
};

#endif