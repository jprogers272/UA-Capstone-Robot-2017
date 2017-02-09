#include "analog.hpp"
#include "DCmotor.hpp"
#include "gpio.hpp"
#include "i2cbus.hpp"
#include "imu.hpp"
#include "ir.hpp"
#include "pwm.hpp"

int main(void) {
//	I2Cbus i2c_bus(2);
//	IMU imu(&i2c_bus);
	
	PWM new_pwm(PWM1A,50000,0,0);
	DCmotor wheel_4(&new_pwm,89);
	
	wheel_4.setVoltage(0.0,12.6);
	wheel_4.off();

		
//	while (1) {
		//imu.printData();
//	}
	
}
