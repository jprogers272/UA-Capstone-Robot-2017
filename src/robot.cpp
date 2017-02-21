#include "robot.hpp"
#include "angleControl.hpp"
#include "i2c_bus.hpp"
#include "robotDefinitions.hpp"
#include "sensors.hpp"

Robot::Robot() {
	I2Cbus i2c_bus(2);
	Sensors sensors(&i2c_bus);
	
	AngleControl angle_controller(PGAIN,IGAIN,DGAIN);
	
	PWM wheel_1_pwm(WHEEL1_PWM,50000,0,0);
	PWM wheel_2_pwm(WHEEL2_PWM,50000,0,0);
	PWM wheel_3_pwm(WHEEL3_PWM,50000,0,0);
	PWM wheel_4_pwm(WHEEL4_PWM,50000,0,0);
	PWM slapper_pwm(SLAPPER_PWM,50000,0,0);
	
	wheel_1(wheel_1_pwm,WHEEL1_DIR);
	wheel_2(wheel_2_pwm,WHEEL2_DIR);
	wheel_3(wheel_3_pwm,WHEEL3_DIR);
	wheel_4(wheel_4_pwm,WHEEL4_DIR);
	slapper(slapper_pwm,SLAPPER_DIR);
	
	drive_voltages = new float[4];
	zeroVoltages();
}

int Robot::robotLogic(void) {
	switch (currentState) {
		case zero_gyro:
			break;
		case pre_stage1:
			break;
		case stage1:
			break;
		case post_stage1:
			break;
		case pre_stage2:
			break;
		case average_compass:
			break;
		case stage2:
			break;
		case post_stage2:
			break;
		case pre_stage3:
			break;
		case stage3:
			break;
		case post_stage3:
			break;
		case pre_stage4:
			break;
		case stage4:
			break;
		case finish:
			return 1;
			break;
	}
	return 0;
}

void Robot::setOutputs(void) {
	wheel_1.setVoltage(drive_voltages[0]);
	wheel_2.setVoltage(drive_voltages[1]);
	wheel_3.setVoltage(drive_voltages[2]);
	wheel_4.setVoltage(drive_voltages[3]);
	slapper.setVoltage(slapper_voltage);
}

void Robot::zeroVoltages(void) {
	drive_voltages[0] = 0.0;
	drive_voltages[1] = 0.0;
	drive_voltages[2] = 0.0;
	drive_voltages[3] = 0.0;
	slapper_voltage = 0.0;
}