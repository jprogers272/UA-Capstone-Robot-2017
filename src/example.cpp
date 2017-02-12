#include "analog.hpp"
#include "compass.hpp"
#include "DCmotor.hpp"
#include "gpio.hpp"
#include "i2cbus.hpp"
#include "imu.hpp"
#include "ir.hpp"
#include "mecanum.hpp"
#include "pwm.hpp"

#include <iostream>

int main(void) {
	I2Cbus i2c_bus(2);
	IMU imu(&i2c_bus);
	Compass compass(&i2c_bus);
	
	PWM pwm1a(PWM1A,50000,0,0);
	PWM pwm1b(PWM1B,50000,0,0);
	PWM pwm2a(PWM2A,50000,0,0);
	PWM pwm2b(PWM2B,50000,0,0);
	
	DCmotor wheel_1(&pwm2b,73);
	DCmotor wheel_2(&pwm2a,75);
	DCmotor wheel_3(&pwm1b,11);
	DCmotor wheel_4(&pwm1a,89);

	wheel_1.setVoltage(3.0,getBatteryVoltage());

	int i = 0;
	while (i < 1000) {
		//imu.printData();
		std::cout << "Compass: " << compass.getAngleRaw() << '\n';
		i++;
	}
	wheel_1.off();
	wheel_2.setVoltage(3.0,getBatteryVoltage());
	i = 0;
	while (i < 100000000) {
		//std::cout << "Compass: " << compass.getAngleRaw() << '\n';
		i++;
	}
	wheel_2.off();
	wheel_3.setVoltage(3.0,getBatteryVoltage());
	i = 0;
	while (i < 100000000) {
		//std::cout << "Compass: " << compass.getAngleRaw() << '\n';
		i++;
	}
	wheel_3.off();
	wheel_4.setVoltage(3.0,getBatteryVoltage());
	i = 0;
	while (i < 100000000) {
		//std::cout << "Compass: " << compass.getAngleRaw() << '\n';
		i++;
	}
	wheel_4.off();

	
	float *voltages = new float[4];
	voltages[0] = 0.0;
	voltages[1] = 0.0;
	voltages[2] = 0.0;
	voltages[3] = 0.0;
	addRotation(voltages,0.5,3.0);
	wheel_1.setVoltage(voltages[0],getBatteryVoltage());
	wheel_2.setVoltage(voltages[1],getBatteryVoltage());
	wheel_3.setVoltage(voltages[2],getBatteryVoltage());
	wheel_4.setVoltage(voltages[3],getBatteryVoltage());

	i = 0;
	while (i < 500000000) { i++; }

	wheel_1.off();
	wheel_2.off();
	wheel_3.off();
	wheel_4.off();
	delete(voltages);
}
