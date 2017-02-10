#include "PIDcontrol.hpp"

PIDcontroller::PIDcontroller(float setpoint, float gainP, float gainI, float gainD) :
	setpoint(setPoint), gainP(gainP), gainI(gainI), gainD(gainD),
	error(0.0), error_prev(0.0), integral(0.0), output(0.0)
{ }

PIDcontroller::calculateOutput(float plant_value, float time_cur) {
	float time_elapsed = time - time_prev;
	time_prev = time_cur;
	error = setpoint - plant_value;
	integral += error * time_elapsed;
	derivative = (error - error_prev) / time_elapsed;
	error_prev = error;
	output = (error * gainP) + (integral * gainI) + (derivative * gainD);
	return output;
}

PIDcontroller::setSetpoint(float setpoint) {
	this->setPoint = setpoint;
}

PIcontroller::PIcontroller(float setpoint, float gainP, float gainI) :
	PIDcontroller(setpoint,gainP,gainI,0.0) 
{ }

PIcontroller::calculateOutput(float plant_value, float time_cur) {
	float time_elapsed = time - time_prev;
	time_prev = time_cur;
	error = setpoint - plant_value;
	integral += error * time_elapsed;
	error_prev = error;
	output = (error * gainP) + (integral * gainI);
	return output;
}