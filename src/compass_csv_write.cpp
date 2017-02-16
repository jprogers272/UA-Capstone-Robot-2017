#include "i2cbus.hpp"
#include "compass.hpp"
#include "timing.hpp"

#include <fstream>
#include <iostream>

using namespace std;

char *file_path = "/home/compass.csv";

int main(void) {
	I2Cbus i2c_bus(2);
	Compass compass(&i2c_bus);
	
	ofstream csv_file;
	csv_file.open(file_path);
	csv_file << "time(ms),time(s),compass\n";
	csv_file.close();
	
	RobotTimer timer;
	timer.start();
	long time_cur;
	float compass_value;
	while (1) {
		compass_value = compass.getAngleF();
		time_cur = timer.getTimeElapsed(PRECISION_MS);
		
		csv_file.open(file_path,ios::app);
		csv_file << time_cur << "," << (time_cur/1000.0) << ',' << compass_value << '\n';
		csv_file.close();
		
		robotWait(0,10);
	}
}
