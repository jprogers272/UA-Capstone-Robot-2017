#include "angleControl.hpp"
#include "pid.hpp"

AngleControl::AngleControl() : PIcontroller(0.0,0.1,0.1,1.0,-1.0), angle(0), average_dps(0) {
	openFileCSV(string("/home/angle_pid.csv"));
}

float AngleControl::calculateRotation(float gyro_dps, int time_cur) {
	calculateAngle(gyro_dps,time_cur);
	calculateOutput(angle,time_cur);
	printFileCSV();
	return output;
}

void AngleControl::setAverage(float average_dps) {
	this->average_dps = average_dps;
}

void AngleControl::calculateAngle(float gyro_dps, int time_cur) {
	//integrate rotational velocity to get angle
	//time is in milliseconds, so divide by 1000
	float time_elapsed = (float)(time_cur - time_prev) / 1000.0;
	angle += (gyro_dps - average_dps) * time_elapsed;
}