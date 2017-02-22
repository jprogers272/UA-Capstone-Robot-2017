#include "robot.hpp"
#include "angleControl.hpp"
#include "i2cbus.hpp"
#include "robotDefinitions.hpp"
#include "sensors.hpp"
#include "timing.hpp"

Robot::Robot() : 
	wheel_1(WHEEL1_PWM,WHEEL1_DIR),
	wheel_2(WHEEL2_PWM,WHEEL2_DIR),
	wheel_3(WHEEL3_PWM,WHEEL3_DIR),
	wheel_4(WHEEL4_PWM,WHEEL4_DIR),
	slapper(SLAPPER_PWM,SLAPPER_DIR)
{
	I2Cbus i2c_bus(2);
	Sensors sensors(&i2c_bus);
	
	AngleControl angle_controller(PGAIN,IGAIN,DGAIN);
	
	/*
	PWM wheel_1_pwm(WHEEL1_PWM,50000,0,0);
	PWM wheel_2_pwm(WHEEL2_PWM,50000,0,0);
	PWM wheel_3_pwm(WHEEL3_PWM,50000,0,0);
	PWM wheel_4_pwm(WHEEL4_PWM,50000,0,0);
	PWM slapper_pwm(SLAPPER_PWM,50000,0,0);
	
	wheel_1 = DCmotor(&wheel_1_pwm,WHEEL1_DIR);
	DCmotor wheel_2(&wheel_2_pwm,WHEEL2_DIR);
	DCmotor wheel_3(&wheel_3_pwm,WHEEL3_DIR);
	DCmotor wheel_4(&wheel_4_pwm,WHEEL4_DIR);
	DCmotor slapper(&slapper_pwm,SLAPPER_DIR);*/
	
	drive_voltages = new float[4];
	zeroVoltages();
}

int Robot::robotLogic(void) {
	switch (currentState) {
		case start:
			start_logic();
			break;
		case zero_gyro:
			zero_gyro_logic();
			break;
		case pre_stage1:
			pre_stage1_logic();
			break;
		case stage1:
			stage1_logic();
			break;
		case post_stage1:
			post_stage1_logic();
			break;
		case pre_stage2:
			pre_stage2_logic();
			break;
		case average_compass:
			average_compass_logic();
			break;
		case stage2:
			stage2_logic();
			break;
		case post_stage2:
			post_stage2_logic();
			break;
		case pre_stage3:
			pre_stage3_logic();
			break;
		case stage3:
			stage3_logic();
			break;
		case post_stage3:
			post_stage3_logic();
			break;
		case pre_stage4:
			pre_stage4_logic();
			break;
		case stage4:
			stage4_logic();
			break;
		case finish:
			return 1;
			break;
	}
	robotWait(0,LOOP_TIME_MS);
	return 0;
}

void Robot::setOutputs(void) {
	wheel_1.setVoltage(drive_voltages[0],sensorData->battery_voltage);
	wheel_2.setVoltage(drive_voltages[1],sensorData->battery_voltage);
	wheel_3.setVoltage(drive_voltages[2],sensorData->battery_voltage);
	wheel_4.setVoltage(drive_voltages[3],sensorData->battery_voltage);
	slapper.setVoltage(slapper_voltage,sensorData->battery_voltage);
}

void Robot::zeroVoltages(void) {
	drive_voltages[0] = 0.0;
	drive_voltages[1] = 0.0;
	drive_voltages[2] = 0.0;
	drive_voltages[3] = 0.0;
	slapper_voltage = 0.0;
}