#include "analog.hpp"
#include "DCmotor.hpp"
#include "gpio.hpp"
#include "i2cbus.hpp"
#include "imu.hpp"
#include "ir.hpp"
#include "pwm.hpp"

int main(void) {
	I2Cbus i2c_bus(2);
	IMU imu(&i2c_bus);
	
	PWM new_pwm(PWM1A,50000,0,0);
	DCmotor wheel_1(&new_pwm);
	
	wheel_1.setVoltage(3.0);
	
	while (1) {
		imu.printData();
	}
}