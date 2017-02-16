#include "robot.hpp"
#include "mecanum.hpp"
#include "sensors.hpp"
#include "timing.hpp"

#include <iostream>

using namespace std;

void Robot::drive_logic(void) {
	rotation = pid_multiplier * angle_controller.calculateRotation(sensorData->gyroZ,timer.getTimeElapsed(PRECISION_MS));
	processMecanum(drive_voltages,voltage_max,translation,translation_angle,rotation);
	cout << "drive voltages are " << drive_voltages[0] << ", " <<
		drive_voltages[1] << ", " <<
		drive_voltages[2] << ", " <<
		drive_voltages[3] << endl;
	cout << "maximum voltage is " << voltage_max << endl;
	cout << "translation multiplier is " << translation << endl;
	cout << "translation angle is " << translation_angle << endl;
	cout << "rotation multipler is " << rotation << endl;
}

void Robot::start_logic(void) {
	currentState = zero_gyro;
	stateLoopCount = 0;
	timer.start();
}

void Robot::zero_gyro_logic(void) {
	cout << "gyroAverage is " << gyroAverage << endl;
	if (stateLoopCount < 50) {
		gyroAverage += sensorData->gyroZ;
		stateLoopCount++;
	}
	else {
		gyroAverage /= 50.0;
		angle_controller.setAverage(gyroAverage);
		stateLoopCount = 0;
		currentState = nextState;
		cout << "measuring took " << timer.getTimeElapsed(PRECISION_MS) << " ms.\n";
		cout << "average is " << gyroAverage << endl;
	}
}

void Robot::pre_stage1_logic(void) {
	
}

void Robot::stage1_logic(void) {
	
}

void Robot::post_stage1_logic(void) {
	
}

void Robot::pre_stage2_logic(void) {
	
}

void Robot::average_compass_logic(void) {
	
}

void Robot::stage2_logic(void) {
	
}

void Robot::post_stage2_logic(void) {
	
}

void Robot::pre_stage3_logic(void) {
	cout << "state loop count is " << stateLoopCount << endl;
	if (stateLoopCount == 0) {
		voltage_max = 3.5;
		translation = 1.0;
		translation_angle = 90.0;
		pid_multiplier = -0.4;
		state_timer.start();
	}
	cout << "state timer is " << state_timer.getTimeElapsed(PRECISION_MS) << "ms\n";
	
		switch (inner_state) {
			case 0:
				if (state_timer.getTimeElapsed(PRECISION_S) < 2) {
					cout << "waiting for state timer\n";
				}
				else {inner_state = 1;}
				break;
			case 1:
				if (sensorData->ir2_4_state == 0) {
					voltage_max = 2.5;
					inner_state = 2;
				}
				break;
			case 2:
				if (sensorData->ir2_3_state == 0) {
					voltage_max = 3.0;
					translation_angle = 0.0;
					inner_state = 3;
				}
				break;
			case 3:
				if ((sensorData->ir2_1_state == 1) && (sensorData->ir2_2_state == 1)) {
					voltage_max = 2.0;
					translation_angle = -180.0;
					state_timer.start();
					inner_state = 4;
				}
				break;
			case 4:
				cout << state_timer.getTimeElapsed(PRECISION_MS) << endl;
				if (state_timer.getTimeElapsed(PRECISION_MS) > 50) {
					cout << "timer is greater than 250 ms\n";
					voltage_max = 0.0;
					inner_state = 0;
					currentState = finish;
				}
				break;
		}
	drive_logic();
	stateLoopCount++;
}

void Robot::stage3_logic(void) {
	
}

void Robot::post_stage3_logic(void) {
	
}

void Robot::pre_stage4_logic(void) {
	
}

void Robot::stage4_logic(void) {
	
}
