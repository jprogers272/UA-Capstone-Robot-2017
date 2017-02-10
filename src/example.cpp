#include "analog.hpp"
#include "compass.hpp"
#include "DCmotor.hpp"
#include "gpio.hpp"
#include "i2cbus.hpp"
#include "imu.hpp"
#include "ir.hpp"
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
	
	DCmotor wheel_1a(&pwm1a,89);
	DCmotor wheel_1b(&pwm1b,11);
	DCmotor wheel_2a(&pwm2a,75);
	DCmotor_wheel_2b(&pwm2b,73);

	i = 0;
	while (i < 100000000) {
		imu.printData();
		std::cout << "Compass: " << compass.getAngleRaw() << '\n';
		wheel_1a.setVoltage(3.0,12.6);
		i++;
	}
	wheel_1a.off();
}
