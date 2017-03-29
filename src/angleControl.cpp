//Author(s): John Rogers

#include "angleControl.hpp"
#include "pid.hpp"
#include "robotDefinitions.hpp"

#include <iostream>
#include <string>

using namespace std;

AngleControl::AngleControl(float gainP, float gainI, float gainD) : PIDcontroller(0.0,gainP,gainI,gainD,0.3,-0.3), angle(0), average_dps(0) {
//	openFileCSV(std::string("/home/angle_pid.csv"));
}

float AngleControl::calculateRotation(float gyro_dps, int time_cur) {
//	cout << "current,previous,difference: " << time_cur << ", " << time_prev << ", " << time_cur-time_prev << endl;
	calculateAngle(gyro_dps,time_cur);
	calculateOutput(angle,time_cur);
//	printFileCSV();
	return output;
}

void AngleControl::setAverage(float average_dps) {
	this->average_dps = average_dps;
}

float AngleControl::getAngle(void) {
	return this->angle;
}

void AngleControl::setAngle(float angle) {
	this->angle = angle;
}

void AngleControl::calculateAngle(float gyro_dps, int time_cur) {
	//integrate rotational velocity to get angle
	//time is in milliseconds, so divide by 1000
	float time_elapsed = (float)(time_cur - time_prev) / 1000.0;
	cout << "time elapsed is " << time_elapsed << " seconds" << endl;
	cout << "dps, average, difference: " << gyro_dps << ", " << average_dps << ", " << (gyro_dps - average_dps) << endl;
	angle += ((gyro_dps - average_dps) * time_elapsed);
	cout << "angle is " << angle << endl;
}
