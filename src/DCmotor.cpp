#include "robotDefinitions.hpp"
#include "DCmotor.hpp"
#include "gpio.hpp"
#include "pwm.hpp"

DCmotor::DCmotor(PWMchannel channel, int directionGPIO) : 
	motorPWM(channel,PWM_PERIOD,0,0), 
	directionGPIO(directionGPIO) 
{
	//set direction pin to output - fix confusing name?
	setDirectionGPIO(directionGPIO,0);
}

void DCmotor::setVoltage(float voltage, float vbat) {
	int motor_direction;
	int duty = 0;
	
	if (voltage > 0) {
		motor_direction = 0;
	}
	else {
		motor_direction = 1;
		voltage *= -1.0;
	}
	duty = (int) (voltage / vbat * PWM_PERIOD);
	//set a minimum duty cycle so motors turn off even with minor floating point errors when voltage = 0.0
	if (duty < 50) {
		duty = 0;
	}
	motorPWM.setDutyCycle(duty);
	writeGPIO(directionGPIO, motor_direction);
	motorPWM.setState(1);
}

void DCmotor::off(void) {
	motorPWM.setDutyCycle(0);
	writeGPIO(directionGPIO,GPIO_OUTPUT);
}
