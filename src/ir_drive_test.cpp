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

int main(void) {
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

	IR ir1_1(8);
	IR ir1_2(76);
	IR ir2_1(78);
	IR ir2_2(86);
	IR ir3_1(87);
	IR ir3_2(10);

	float *voltages = new float[4];
	voltages[0] = 0.0;
	voltages[1] = 0.0;
	voltages[2] = 0.0;
	voltages[3] = 0.0;

	float vbat = getBatteryVoltage();

	processMecanum(voltages,3.0,1.0,-90.0,0.0);
	voltages[2] += 0.15;
	cout << "motor voltages: " << voltages[0] << "," << voltages[1] << "," << voltages[2] << "," << voltages[3] << endl;
	wheel_1.setVoltage(voltages[0],vbat);
	wheel_2.setVoltage(voltages[1],vbat);
	wheel_3.setVoltage(voltages[2],vbat);
	wheel_4.setVoltage(voltages[3],vbat);

	std::cout << "strafing toward stage1\n";
	while (1) {
		if (!ir1_1.getData() && !ir1_2.getData())
			break;
		robotWait(0,10);
	}

	std::cout << "driving toward stage2 wall\n";

	vbat = getBatteryVoltage();
	processMecanum(voltages,2.5,1.0,0.0,0.0);
	cout << "motor voltages: " << voltages[0] << "," << voltages[1] << "," << voltages[2] << "," << voltages[3] << endl;
	wheel_1.setVoltage(voltages[0],vbat);
	wheel_2.setVoltage(voltages[1],vbat);
	wheel_3.setVoltage(voltages[2],vbat);
	wheel_4.setVoltage(voltages[3],vbat);

	while (1) {
		if (!ir2_1.getData() && !ir2_2.getData())
			break;
		robotWait(0,10);
	}

	std::cout << "strafing toward stage3 wall\n";

	vbat = getBatteryVoltage();
	processMecanum(voltages,3.0,1.0,90.0,0.0);
	voltages[2] -= 0.05;
	cout << "motor voltages: " << voltages[0] << "," << voltages[1] << "," << voltages[2] << "," << voltages[3] << endl;
	wheel_1.setVoltage(voltages[0],vbat);
	wheel_2.setVoltage(voltages[1],vbat);
	wheel_3.setVoltage(voltages[2],vbat);
	wheel_4.setVoltage(voltages[3],vbat);

	while (1) {
		if (!ir3_1.getData() && !ir3_2.getData())
			break;
		robotWait(0,10);
	}
	
	wheel_1.off();
	wheel_2.off();
	wheel_3.off();
	wheel_4.off();
}
