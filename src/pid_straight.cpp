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

#include <iostream>

using namespace std;

void averageGyro(IMU*,AngleControl*);
void zeroVoltages(float*);

int time_ms = 0;

int main (void) {
	I2Cbus i2c_bus(2);
	IMU imu(&i2c_bus);
	Compass compass(&i2c_bus);
	AngleControl angle_controller(0.1,0.03,0.005);
	
	/*
	PWM pwm1a(PWM1A,50000,0,0);
	PWM pwm1b(PWM1B,50000,0,0);
	PWM pwm2a(PWM2A,50000,0,0);
	PWM pwm2b(PWM2B,50000,0,0);
	*/
	
	DCmotor wheel_1(PWM2B,73);
	DCmotor wheel_2(PWM2A,75);
	DCmotor wheel_3(PWM1B,11);
	DCmotor wheel_4(PWM1A,89);
	
	averageGyro(&imu,&angle_controller);
	float *voltages = new float[4];
	float rotation = 0.0;
	float translation = -1.0;
	float multiplier = -0.4;
	float vbat;
	zeroVoltages(voltages);
	cout << "voltages: " << voltages[0] << "," << voltages[1] << "," << voltages[2] << "," << voltages[3] << endl;
	RobotTimer timer;
	timer.start();
	while (1) {
		rotation = multiplier * angle_controller.calculateRotation(imu.getGyroZ(),timer.getTimeElapsed(PRECISION_MS));
		//addRotation(voltages,rotation,1.5);
		//processMecanum(voltages,3.0,0.0,0.0,rotation);
		processMecanum(voltages,4.0,translation,-90.0,rotation);
		cout << "voltages: " << voltages[0] << "," << voltages[1] << "," << voltages[2] << "," << voltages[3] << endl;
		vbat = getBatteryVoltage();
		wheel_1.setVoltage(voltages[0],vbat);
		wheel_2.setVoltage(voltages[1],vbat);
		wheel_3.setVoltage(voltages[2],vbat);
		wheel_4.setVoltage(voltages[3],vbat);
		robotWait(0,10); //50ms
		/*
		if (timer.getTimeElapsed(PRECISION_S) > 15) {
			angle_controller.setSetpoint(90.0);
		}
		if (timer.getTimeElapsed(PRECISION_S) > 35) {
			break;
		}
		*/
		if (timer.getTimeElapsed(PRECISION_MS) > 2730) {
			break;
		}
		
	}

	wheel_1.off();
	wheel_2.off();
	wheel_3.off();
	wheel_4.off();
}

void averageGyro(IMU *imu, AngleControl *angle_controller) {
	float dps_sum = 0.0;
	int i = 0;
	RobotTimer timer;
	timer.start();
	while (i < 50) {
		dps_sum += imu->getGyroZ();
		robotWait(0,20);
		i++;
	}
	cout << "measuring took " << timer.getTimeElapsed(PRECISION_MS) << "ms" << endl;
	float average = dps_sum / (float)(i+1);
	angle_controller->setAverage(average);
	cout << "average set to " << average << endl;
}

void zeroVoltages(float *voltages) {
	voltages[0] = 0.0;
	voltages[1] = 0.0;
	voltages[2] = 0.0;
	voltages[3] = 0.0;
}
