#include "pid.hpp"

#include <string>
#include <fstream>

using namespace std;

PIcontroller::PIcontroller(float setpoint, float gainP, float gainI) :
	setpoint(setpoint), gainP(gainP), gainI(gainI),
	error(0.0), error_prev(0.0), integral(0.0), output(0.0)
{ }

float PIcontroller::calculateOutput(float plant_value, float time_cur) {
	float time_elapsed = time_cur - time_prev;
	time_prev = time_cur;
	error = setpoint - plant_value;
	integral += error * time_elapsed;
	error_prev = error;
	output = (error * gainP) + (integral * gainI);
	return output;
}

void PIcontroller::setSetpoint(float setpoint) {
	this->setpoint = setpoint;
}

void PIcontroller::openFileCSV(string fileCSV) {
	this->fileCSV = fileCSV;
	ofstream out_file;
	out_file.open(this->fileCSV.c_str(), fstream::app);
	out_file << "Unix Time,time,Setpoint,Error,Output,Plant\n";
	out_file.close();
}

void PIcontroller::printFileCSV() {
	ofstream out_file;
	out_file.open(fileCSV.c_str(), fstream::app);
	out_file << /*unix time*/",";
	out_file << time_prev << ',' << setpoint << ',' << error 
		<< ',' << output << ',' << plant_value << '\n';
	out_file.close();
}

PIDcontroller::PIDcontroller(float setpoint, float gainP, float gainI, float gainD) :
	PIcontroller(setpoint,gainP,gainI) 
{ }

float PIDcontroller::calculateOutput(float plant_value, float time_cur) {
	float time_elapsed = time_cur - time_prev;
	time_prev = time_cur;
	error = setpoint - plant_value;
	integral += error * time_elapsed;
	derivative = (error - error_prev) / time_elapsed;
	error_prev = error;
	output = (error * gainP) + (integral * gainI) + (derivative * gainD);
	return output;
}