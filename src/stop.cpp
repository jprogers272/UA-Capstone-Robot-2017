#include <iostream>
#include "DCmotor.hpp"
#include "pwm.hpp"

using namespace std;

int main()
{
	/*
	PWM pwm1a(PWM1A,50000,0,0);
	PWM pwm1b(PWM1B,50000,0,0);
	PWM pwm2a(PWM2A,50000,0,0);
	PWM pwm2b(PWM2B,50000,0,0);
	PWM pwm0a(PWM0A,50000,0,0);
	*/
	DCmotor wheel_1(PWM2B,73);
	DCmotor wheel_2(PWM2A,75);
	DCmotor wheel_3(PWM1B,11);
	DCmotor wheel_4(PWM1A,89);
	DCmotor slapper(PWM0A,72);

	wheel_1.off();
	wheel_2.off();
	wheel_3.off();
	wheel_4.off();
	slapper.off();
}
