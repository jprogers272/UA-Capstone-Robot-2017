//Author(s): William Khan

#include <iostream>
#include <vector>
#include <time.h>
#include "compass.hpp"
#include "i2cbus.hpp"
#include "DCmotor.hpp"
#include "pwm.hpp"

using namespace std;

float Average(vector<float> const &numbers);

int main()
{
	I2Cbus i2c_bus(2);
	Compass compass(&i2c_bus);
	//PWM pwm0a(PWM0A,50000,0,0);
	DCmotor slapper(PWM0A,72);

	
	 vector<float> initReadings;

	for (int i = 0; i < 500; ++i)
	{
		initReadings.push_back(compass.getAngleF());
	}

	float initAvg = Average(initReadings);
	cout << endl << "Average Initial Reading: " << initAvg << endl;

	float temp = 0;
	while(1)
	{
		temp = compass.getAngleF();
		cout << "Present Reading " <<temp << endl;
		if((temp - initAvg) > 10 || (temp - initAvg) < -10)
		{
			cout << "Stopping Motors " << temp-initAvg << endl;
			slapper.off();
		}
		else
		{
			cout << "Running motors " << temp-initAvg << endl;
			slapper.setVoltage(4.5,12.6);
		}
	}	
	return 0;
}

float Average(vector<float> const &numbers)
{
	float sum = 0;
	int totalvals = numbers.size();

	for (int i = 0; i < totalvals; ++i)
	{
		sum += numbers.at(i);
	}
	
	return sum / totalvals;
}
