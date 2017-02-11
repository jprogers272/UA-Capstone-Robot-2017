#include <iostream>
#include "DCmotor.hpp"
#include "pwm.hpp"

using namespace std;

int main()
{
	
	PWM pwm1a(PWM1A,50000,0,0);
	PWM pwm1b(PWM1B,50000,0,0);
	PWM pwm2a(PWM2A,50000,0,0);
	PWM pwm2b(PWM2B,50000,0,0);
	PWM pwm0a(PWM0A,50000,0,0);
	DCmotor wheel_1(&pwm2b,73);
	DCmotor wheel_2(&pwm2a,75);
	DCmotor wheel_3(&pwm1b,11);
	DCmotor wheel_4(&pwm1a,89);
	DCmotor slapper(&pwm0a,72);

	wheel_1.off();
	wheel_2.off();
	wheel_3.off();
    wheel_4.off();
    slapper.off();

}