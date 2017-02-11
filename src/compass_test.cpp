#include <iostream>
#include <time.h>
#include <vector>
#include "compass.hpp"
#include "i2cbus.hpp"
using namespace std;

int main()
{
	I2Cbus i2c_bus(2);
	Compass compass(&i2c_bus);

	
	int count = 0;

	vector<float> init_field_readings;

	while(count < 100)
	{
		init_field_readings.push_back(compass.getAngleF());
		count++;
	}

	int total_readings = init_field_readings.size();
	float initAvg = 0, sum = 0;
	
	for (int i = 0; i < total_readings; ++i)
	{
		sum +=init_field_readings.at(i);
	}
	initAvg = sum / total_readings;

	cout << "Average initial reading " << initAvg << endl
 		 << "Total number of readings " << total_readings << endl;


	/*Delay Stuff
	clock_t time;

	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;*/
	
	return 0;
}
