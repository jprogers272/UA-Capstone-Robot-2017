#include "pid.hpp"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

PIcontroller::PIcontroller(float setpoint, float gainP, float gainI, float limit_upper, float limit_lower) :
	setpoint(setpoint), gainP(gainP), gainI(gainI), limit_upper(limit_upper), limit_lower(limit_lower),
	error(0.0), error_prev(0.0), integral(0.0), output(0.0), time_prev(0)
{ }

void PIcontroller::calculateOutput(float plant_value, int time_cur) {
	float time_elapsed = (float)(time_cur - time_prev) / 1000.0;
//	cout << "time_prev: " << time_prev << endl;
	time_prev = time_cur;
//	cout << "time_prev: " << time_prev << endl;
	this->plant_value = plant_value;
	error = setpoint - this->plant_value;
//	std::cout << "error is " << error << endl;
	integral += error * time_elapsed;
	error_prev = error;
	output = (error * gainP) + (integral * gainI);
	limitOutput();
}

void PIcontroller::setSetpoint(float setpoint) {
	this->setpoint = setpoint;
}

float PIcontroller::getOutput(void) {
	return output;
}

void PIcontroller::openFileCSV(string fileCSV) {
	this->fileCSV = fileCSV;
	ofstream out_file;
	out_file.open(this->fileCSV.c_str(), fstream::app);
	out_file << "Unix Time,time,Setpoint,Error,Output,Plant\n";
	out_file.close();
}

void PIcontroller::printFileCSV(void) {
	ofstream out_file;
	out_file.open(fileCSV.c_str(), fstream::app);
	out_file << /*unix time*/",";
	out_file << time_prev << ',' << setpoint << ',' << error 
		<< ',' << output << ',' << plant_value << '\n';
	out_file.close();
}

void PIcontroller::limitOutput(void) {
	if (output > limit_upper) {
		output = limit_upper;
	}
	else if (output < limit_lower) {
		output = limit_lower;
	}
}

PIDcontroller::PIDcontroller(float setpoint, float gainP, float gainI, float gainD, float limit_upper, float limit_lower) :
	PIcontroller(setpoint,gainP,gainI,limit_upper,limit_lower), gainD(gainD)
{ }

void PIDcontroller::calculateOutput(float plant_value, float time_cur) {
	float time_elapsed = (float)(time_cur - time_prev) / 1000.0;
	time_prev = time_cur;
	this->plant_value = plant_value;
	error = setpoint - this->plant_value;
	integral += error * time_elapsed;
	derivative = (error - error_prev) / time_elapsed;
	error_prev = error;
	output = (error * gainP) + (integral * gainI) + (derivative * gainD);
	limitOutput();
}
