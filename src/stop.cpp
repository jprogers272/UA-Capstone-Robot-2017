//Author(s): William Khan
#include "robotDefinitions.hpp"
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
	DCmotor wheel_1(WHEEL1_PWM,WHEEL1_DIR);
	DCmotor wheel_2(WHEEL2_PWM,WHEEL2_DIR);
	DCmotor wheel_3(WHEEL3_PWM,WHEEL3_DIR);
	DCmotor wheel_4(WHEEL4_PWM,WHEEL4_DIR);
	DCmotor slapper(SLAPPER_PWM,SLAPPER_DIR);

	wheel_1.off();
	wheel_2.off();
	wheel_3.off();
	wheel_4.off();
	slapper.off();
}
