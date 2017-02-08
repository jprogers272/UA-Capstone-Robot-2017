#include "DCmotor.hpp"
#include "gpio.hpp"
#include "pwm.hpp"

using namespace std;

DCmotor::DCmotor(PWM *motorPWM, int directionGPIO) : motorPWM(motorPWM), directionGPIO(directionGPIO) {
}

void DCmotor::setVoltage(float voltage, float vbat) {
	int motor_direction = 0;
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
	motorPWM->setDutyCycle(duty);
	writeGPIO(directionGPIO, motor_direction);
	motorPWM->setState(1);
}

void DCmotor::off(void) {
	motorPWM->setState(0);
	writeGPIO(directionGPIO,0);
}