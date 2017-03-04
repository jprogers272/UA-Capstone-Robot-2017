//Author(s): John Rogers

#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "angleControl.hpp"
#include "DCmotor.hpp"
#include "i2cbus.hpp"
#include "sensors.hpp"
#include "stage1.hpp"
#include "timing.hpp"
#include "display.hpp"
#include "positionTracker.hpp"

enum State {
	start,
	zero_gyro,
	pre_stage1,
	stage1_solving,
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
		DCmotor wheel_1;
		DCmotor wheel_2;
		DCmotor wheel_3;
		DCmotor wheel_4;
		DCmotor slapper;
		AngleControl angle_controller;
		PositionTracker position_tracker;
		I2Cbus i2c_bus;
		Sensors sensors;
		SensorData *sensorData;
		State currentState;
		State nextState;
		int stateLoopCount;
		float gyroAverageZ;
		float gyroAverageY;
		float gyroAverageX;
		float compAverage;
		float *drive_voltages;
		float slapper_voltage;
		float voltage_max;
		float translation;
		float translation_angle;
		float rotation;
		float pid_multiplier;
		RobotTimer timer;
		RobotTimer state_timer;
		int inner_state;
		Stage1 stage1;
	
	//member functions
	public:
		Robot(void);
		int robotLogic(void);
		void setOutputs(void);
	
	private:
		void zeroVoltages(void);
		
		void drive_logic(void);
		void setDriveDirection(int,float);
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
		void finish_logic(void);
};

#endif
