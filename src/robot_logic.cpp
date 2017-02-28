//Author(s): John Rogers, tbd

#include "robot.hpp"
#include "mecanum.hpp"
#include "sensors.hpp"
#include "stage1.hpp"
#include "timing.hpp"

#include <iostream>

using namespace std;

void Robot::drive_logic(void) {
	rotation = pid_multiplier * angle_controller.calculateRotation(sensorData->gyroZ,timer.getTimeElapsed(PRECISION_MS));
	processMecanum(drive_voltages,voltage_max,translation,translation_angle,rotation*3.0/voltage_max);
	cout << "drive voltages are " << drive_voltages[0] << ", " <<
		drive_voltages[1] << ", " <<
		drive_voltages[2] << ", " <<
		drive_voltages[3] << endl;
	cout << "maximum voltage is " << voltage_max << endl;
	cout << "translation multiplier is " << translation << endl;
	cout << "translation angle is " << translation_angle << endl;
	cout << "rotation multipler is " << rotation << endl;
}

void Robot::setDriveDirection(int direction, float voltage) {
	voltage_max = voltage;
	if (direction == STRAIGHT_FORWARD) {
		translation = 1.0;
		translation_angle = 0.0;
		pid_multiplier = -1.0;
	}
	else if (direction == STRAIGHT_BACKWARD) {
		translation = 1.0;
		translation_angle = -180.0;
		pid_multiplier = -1.0;
	}
	else if (direction == STRAFE_LEFT) {
		translation = 1.0;
		translation_angle = -90.0;
		pid_multiplier = -0.4;
	}
	else if (direction == STRAFE_RIGHT) {
		translation = 1.0;
		translation_angle = 90.0;
		pid_multiplier = -0.4;
	}
	else {//direction == STOPPED
		translation = 0.0;
		translation_angle = 0.0;
		pid_multiplier = 0.0;
		voltage_max = 0.0;
	}
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
	cout << "inner state is " << inner_state << endl;
	switch (inner_state) {
		case 0:
			state_timer.start();
			setDriveDirection(STRAIGHT_BACKWARD,3.0);
			inner_state++;
			break;
		case 1:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 750) {
				setDriveDirection(STRAFE_LEFT,3.5);
				inner_state++;
			}
			break;
		case 2:
			if ((sensorData->ir1_1_state == 0) && (sensorData->ir1_2_state == 0)) {
				setDriveDirection(STRAFE_RIGHT,3.5);
				state_timer.start();
				inner_state++;
			}
			break;
		case 3:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 100) {
				setDriveDirection(STRAIGHT_FORWARD,3.0);
				inner_state++;
			}
			break;
		case 4:
			if (sensorData->ir1_3_state == 0) {
				setDriveDirection(STRAIGHT_FORWARD,2.0);
				inner_state++;
			}
			break;
		case 5:
			if ((sensorData->ir1_4_state == 0) && (sensorData->ir1_3_state == 1)) {
				setDriveDirection(STRAFE_LEFT,2.5);
				inner_state = 0;
				currentState = stage1_solving;
			}
			break;
	}
	drive_logic();
}

void Robot::stage1_logic(void) {
	switch (inner_state) {
		case 0:
			stage1.energizeComponent();
			state_timer.start();
			inner_state++;
			break;
		case 1:
			if ((stage1.currentComponent < 5) && (state_timer.getTimeElapsed(PRECISION_MS) > STAGE1_CHARGING_TIME)) {
				stage1.identifyComponent();
				//cout << "identified component " << stage1.currentComponent << " as " << stage1.components[stage1.currentComponent] << endl;
				stage1.currentComponent++;
				inner_state = 0;
			}
			else {
				stage1.energizeCommon();
				state_timer.start();
				inner_state++;
			}
			break;
		case 2:
			if (state_timer.getTimeElapsed(PRECISION_MS) > STAGE1_CHARGING_TIME) {
				stage1.checkCapacitorDiode();
				if (stage1.detectProblems() == 0) {
					setDriveDirection(STOPPED,0);
					inner_state = 0;
					currentState = post_stage1;
				}
				else {
					inner_state++;
				}
			}
			break;
		case 3:
			cout << "fuck\n";
			cout << "reorient robot?\n";
			if (translation_angle > -85.0) {
				translation_angle = -145.0;
			}
			else if (translation_angle < -95.0) {
				translation_angle = -75.0;
			}
			else {
				//it's 90.0, accounting for floating point error
				translation_angle = -75.0;
			}
		       	state_timer.start();
			inner_state = 4;
			stage1.currentComponent = 0;
			break;
		case 4:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 500) {
				inner_state = 0;
			}
			break;
	}
	drive_logic();
}

void Robot::post_stage1_logic(void) {
	cout << "code is " 
		<< stage1.components[0] << ' '
		<< stage1.components[1] << ' '
		<< stage1.components[2] << ' '
		<< stage1.components[3] << ' '
		<< stage1.components[4] << endl;
		cout << "inner state is " << inner_state << endl;
		switch(inner_state)
		{
			case 0:
				state_timer.start();
				inner_state++;
				break;
			case 1:
				setDriveDirection(STRAFE_RIGHT,3.0);
				if((sensorData->ir1_1_state == 1) && (sensorData->ir1_2_state == 1))
				{
					setDriveDirection(STOPPED,0.0);
					inner_state++;
				}
				break;
			case 2:
				setDriveDirection(STRAIGHT_FORWARD,6.0);
				inner_state++;
				break;
			case 3:
				if((sensorData->ir2_1_state == 0) && (sensorData->ir2_2_state == 0))
				{
					setDriveDirection(STOPPED,0.0);
					inner_state = 0;
					currentState = pre_stage2;
				}
				break;
			default:
				setDriveDirection(STOPPED,0.0);
				currentState = finish;
				break;
		}
	drive_logic();
}

void Robot::pre_stage2_logic(void) {
	cout << "inner state is " << inner_state << endl;
	switch (inner_state) {
		case 0:
			setDriveDirection(STRAFE_RIGHT,3.5);
			inner_state++;
			break;
		case 1:
			if (sensorData->ir2_4_state == 0)
			{
				setDriveDirection(STRAFE_RIGHT,3.0);
				inner_state++;
			}	
			break;
		case 2:
			if (sensorData->ir2_3_state == 0)
			{
				setDriveDirection(STOPPED,0.0);
				inner_state++;
			}
			break;
		case 3:
			setDriveDirection(STRAIGHT_FORWARD,2.0);
			state_timer.start();
			inner_state++;
			break;
		case 4:
			if (state_timer.getTimeElapsed(PRECISION_MS)>200)
			{
				setDriveDirection(STOPPED,0.0);
				inner_state = 0;
				currentState = finish;
			}
			break;
		default:
			setDriveDirection(STOPPED,0.0);
			inner_state = 0;
			currentState = finish;
			break;
	}
	drive_logic();
}


void Robot::average_compass_logic(void) {
	
}

void Robot::stage2_logic(void) {
	
}

void Robot::post_stage2_logic(void) {
	
}

void Robot::pre_stage3_logic(void) {
	if (stateLoopCount == 0) {		
		state_timer.start();
	}
	cout << "state timer is " << state_timer.getTimeElapsed(PRECISION_MS) << "ms\n";	
	switch (inner_state) {
		case 0:
			setDriveDirection(STRAFE_RIGHT,3.5);
			if (state_timer.getTimeElapsed(PRECISION_S) > 2) {
				inner_state = 1;
			}
			break;
		case 1:
			if (sensorData->ir2_4_state == 0) {
				setDriveDirection(STRAFE_RIGHT,2.5);
				inner_state = 2;
			}
			break;
		case 2:
			if (sensorData->ir2_3_state == 0) {
				setDriveDirection(STRAIGHT_FORWARD,3.0);
				inner_state = 3;
			}
			break;
		case 3:
			if ((sensorData->ir2_1_state == 1) && (sensorData->ir2_2_state == 1)) {
				setDriveDirection(STRAIGHT_BACKWARD,2.0);
				state_timer.start();
				inner_state = 4;
			}
			break;
		case 4:
			cout << state_timer.getTimeElapsed(PRECISION_MS) << endl;
			if (state_timer.getTimeElapsed(PRECISION_MS) > 50) {
				setDriveDirection(STOPPED,0.0);
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
