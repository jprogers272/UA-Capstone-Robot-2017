#include "mecanum.hpp"

#include <cmath>

#define PI 3.14159

/*calculates voltages required for each mecanum wheel given 4 parameters, plus a pointer where the voltages will be stored
 * parameters:
 * voltages: the array in which the calculated voltages are stored
 * maximum_voltage: the maximum value any one voltage is allowed to be
 * translation_velocity: a multiplier, between -1.0 and 1.0, that 
 		sets the translation speed
 * translation_angle: between -180.0 and 180.0; specifies at what angle
		the robot ought to move, where 0.0 degrees is straight forward
 * rotation_velocity: a multiplier, between -1.0 and 1.0, that sets the 
		speed at which the robot will rotate around its center
*/
void processMecanum(float *voltages, float maximum_voltage, float translation_velocity, float translation_angle, float rotation_velocity) {
	//possibly comment these checks out later, if we are confident that our program will not try anything weird
	//commenting all because of inclusion of scaling later
//	if (maximum_voltage > 6.0) 
//		maximum_voltage = 6.0;
//	if (maximum_voltage < 0.0) 
//		maximum_voltage = 0.0;
//	if (translation_velocity > 1.0) 
//		translation_velocity = 1.0;
//	else if (translation_velocity < -1.0) 
//		translation_velocity = -1.0;
//	translation_angle *= PI/180.0;
//	if (rotation_velocity > 1.0) 
//		rotation_velocity = 1.0;
//	else if (rotation_velocity < -1.0) 
//		rotation_velocity = -1.0;
	
	voltages[0] = maximum_voltage * (translation_velocity * sin(translation_angle + PI/4.0) + rotation_velocity);
	voltages[1] = maximum_voltage * (translation_velocity * cos(translation_angle + PI/4.0) - rotation_velocity);
	voltages[2] = maximum_voltage * (translation_velocity * cos(translation_angle + PI/4.0) + rotation_velocity);
	voltages[3] = maximum_voltage * (translation_velocity * sin(translation_angle + PI/4.0) - rotation_velocity);
	
	//check if any voltage magnitudes are greater than the provided maximum, and scale accordingly
	scaleVoltages(voltages,maximum_voltage);
}

void addRotation(float *voltages, float rotation_velocity, float maximum_voltage) {
	voltages[0] += maximum_voltage * rotation_velocity;
	voltages[1] -= maximum_voltage * rotation_velocity;
	voltages[2] += maximum_voltage * rotation_velocity;
	voltages[3] -= maximum_voltage * rotation_velocity;
	scaleVoltages(voltages,maximum_voltage);
}

void scaleVoltages(float *voltages, float maximum_voltage) {
	int i;
	float calculated_max = fabs(voltages[0]);
	for (i=1; i<4; i++) {
		if (fabs(voltages[i]) > calculated_max)
			calculated_max = fabs(voltages[i]);
	}
	if (calculated_max > maximum_voltage) {
		for (i=0; i<4; i++) {
			voltages[i] = voltages[i] * maximum_voltage / calculated_max;
		}
	}
}
