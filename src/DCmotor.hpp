#ifndef DCMOTOR_HPP
#define DCMOTOR_HPP

#include "pwm.hpp"

class DCmotor {
	//members
	private:
		PWM motorPWM;
		int directionGPIO;
	
	//member functions
	public:
		DCmotor(PWMchannel,int);
		void setVoltage(float,float);
		void off(void);
};

#endif