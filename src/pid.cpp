#include "pid.hpp"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

PIDcontroller::PIDcontroller(float setpoint, float gainP, float gainI, float gainD,  float limit_upper, float limit_lower) :
	setpoint(setpoint), gainP(gainP), gainI(gainI), gainD(gainD), limit_upper(limit_upper), limit_lower(limit_lower),
	error(0.0), error_prev(0.0), integral(0.0), output(0.0), time_prev(0)
{ }

void PIDcontroller::calculateOutput(float plant_value, int time_cur) {
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

void PIDcontroller::setSetpoint(float setpoint) {
	this->setpoint = setpoint;
}

float PIDcontroller::getOutput(void) {
	return output;
}

void PIDcontroller::openFileCSV(string fileCSV) {
	this->fileCSV = fileCSV;
	ofstream out_file;
	out_file.open(this->fileCSV.c_str());
	out_file << "Time(ms),Time(s),Setpoint,Error,Integral,Derivative,Output,Plant";
	out_file << ",K_P = " << gainP;
	out_file << ",K_I = " << gainI;
	out_file << ",K_D = " << gainD << '\n';
	out_file.close();
}

void PIDcontroller::printFileCSV(void) {
	ofstream out_file;
	out_file.open(fileCSV.c_str(), fstream::app);
	out_file << time_prev << ',' << (time_prev/1000.0) << ',' 
		<< setpoint << ',' << error << ',' << integral << ','
		<< derivative << ',' << output << ',' 
		<< plant_value << '\n';
	out_file.close();
}

void PIDcontroller::limitOutput(void) {
	if (output > limit_upper) {
		output = limit_upper;
	}
	else if (output < limit_lower) {
		output = limit_lower;
	}
}

