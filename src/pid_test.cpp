#include "analog.hpp"
#include "angleControl.hpp"
#include "battery.hpp"
#include "compass.hpp"
#include "DCmotor.hpp"
#include "gpio.hpp"
#include "i2cbus.hpp"
#include "imu.hpp"
#include "ir.hpp"
#include "mecanum.hpp"
#include "pid.hpp"
#include "pwm.hpp"
#include "timing.hpp"

int time_ms = 0;

int main (void) {
	I2Cbus i2c_bus(2);
	IMU imu(&i2c_bus);
	Compass compass(&i2c_bus);
	AngleControl angle_controller();
	
	PWM pwm1a(PWM1A,50000,0,0);
	PWM pwm1b(PWM1B,50000,0,0);
	PWM pwm2a(PWM2A,50000,0,0);
	PWM pwm2b(PWM2B,50000,0,0);
	
	DCmotor wheel_1(&pwm2b,73);
	DCmotor wheel_2(&pwm2a,75);
	DCmotor wheel_3(&pwm1b,11);
	DCmotor wheel_4(&pwm1a,89);
	
	float *voltages = new float[4];
	zero_voltages(voltages);
	RobotTimer timer();
	timer.start();
	while (1) {
		angle_controller.calculateRotation(imu.getGyroZ,timer.getTimeElapsed(PRECISION_MS));
		robotWait(0,50); //50ms
	}
}

void average_gyro(IMU *imu, AngleControl *angle_controller) {
	float dps_sum = 0.0;
	int i = 0;
	RobotTimer timer();
	timer.start();
	while (i < 50) {
		dps_sum += imu->getGyroZ(void);
		robotWait(0,20);
	}
	cout << "measuring took " timer.getTimeElapsed(PRECISION_MS) << "ms" << endl;
	float average = dps_sum / (float)(i+1);
	angle_controller->setAverage(average);
}

void zero_voltages(float *voltages) {
	voltages[0] = 0.0;
	voltages[1] = 0.0;
	voltages[2] = 0.0;
	voltages[3] = 0.0;
}