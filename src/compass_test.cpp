//Author(s): William Khan

#include <iostream>
#include "robotDefinitions.hpp"
#include "timing.hpp"
#include "compass.hpp"
#include "i2cbus.hpp"
#include "DCmotor.hpp"
#include "pwm.hpp"

using namespace std;

float Average(float sum, int totalvals);
float initialReading(int totReadings);

int main()
{
	DCmotor wheel_1(WHEEL1_PWM,WHEEL1_DIR);
	DCmotor wheel_2(WHEEL2_PWM,WHEEL2_DIR);
	DCmotor wheel_3(WHEEL3_PWM,WHEEL3_DIR);
	DCmotor wheel_4(WHEEL4_PWM,WHEEL4_DIR);
	DCmotor slapper(SLAPPER_PWM,SLAPPER_DIR);

	I2Cbus i2c_bus(2);
	Compass compass(&i2c_bus);
	
//	RobotTimer time;

//	time.start();
//	wheel_1.setVoltage(2.5,12.6);
//	wheel_2.setVoltage(-2.5,12.6);
//	wheel_3.setVoltage(-2.5,12.6);
//	wheel_4.setVoltage(2.5,12.6);
//	while(time.getTimeElapsed(0)<2)
//	{
//		cout << time.getTimeElapsed(0) << endl;
//	}
	wheel_1.off();
	wheel_2.off();
	wheel_3.off();
	wheel_4.off();
	



	// enum state{prestage2,stage2,poststage2};
	// state currentState = prestage2;

	// switch(currentState)
	// {
	// 	case prestage2:

	// 		break;

	// 	case stage2:
	 		float initReading = initialReading(500);
	 		float temp = 0;
	 		while(1)
	 		{
	 			temp = compass.getAngleF();
	 			cout << "Present Reading " <<temp << endl;
	 			if((temp - initReading) > 10 || (temp - initReading) < -10)
	 			{
	 				cout << "Slapping. Field Diff: " << temp-initReading << endl;
	 				slapper.off();
	 			}
	 			else
	 			{
	 				cout << "Stopping. Field Diff: " << temp-initReading << endl;
	 				slapper.setVoltage(4.5,12.6);
	 			}
	 		}
	// 		break;	

	// 	case poststage2:

	// 		break;

	// }



	return 0;
}


float Average(float sum, int totalvals)
{
	return sum / totalvals;
}

float initialReading(int totReadings)
{
	I2Cbus i2c_bus(2);
	Compass compass(&i2c_bus);
	float fieldVal[totReadings];
	float sum = 0;

	for (int i = 0; i < totReadings; ++i)
	{
		fieldVal[i] = compass.getAngleF();
		sum += fieldVal[i];
	}

	return Average(sum, totReadings);
}


