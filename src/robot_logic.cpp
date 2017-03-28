//Author(s): John Rogers, William Khan

#include "robot.hpp"
#include "gpio.hpp"
#include "mecanum.hpp"
#include "positionTracker.hpp"
#include "sensors.hpp"
#include "stage1.hpp"
#include "timing.hpp"

#include <iostream>

using namespace std;

void Robot::drive_logic(void) {
	position_tracker.calculateAnglesOnly(sensorData->gyroX,sensorData->gyroY,sensorData->gyroZ,timer.getTimeElapsed(PRECISION_MS));
	rotation = pid_multiplier * angle_controller.calculateRotation(sensorData->gyroZ,timer.getTimeElapsed(PRECISION_MS));
	processMecanum(drive_voltages,voltage_max,translation,translation_angle,rotation*3.0/voltage_max);
	cout << "drive voltages are " << drive_voltages[0] << ", " <<
		drive_voltages[1] << ", " <<
		drive_voltages[2] << ", " <<
		drive_voltages[3] << endl;
	//cout << "maximum voltage is " << voltage_max << endl;
	//cout << "translation multiplier is " << translation << endl;
	//cout << "translation angle is " << translation_angle << endl;
	//cout << "rotation multipler is " << rotation << endl;
}

void Robot::setDriveDirection(int direction, float voltage) {
	voltage_max = voltage;
	angle_controller.enableIntegral(IGAIN);
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
	else if (direction == ROTATE) {
		angle_controller.disableIntegral();
		translation = 0.0;
		translation = 0.0;
		pid_multiplier = -3.0;
	}
	else {//direction == STOPPED
		translation = 0.0;
		translation_angle = 0.0;
		pid_multiplier = 0.0;
		voltage_max = 0.0;
	}
}

void Robot::start_logic(void) {
    disp.writeCenter("Robot Starting Up...",0);
    disp.writeDisplay();
    currentState = zero_gyro;
	stateLoopCount = 0;
	timer.start();
    disp.clearDisplay();
}

void Robot::zero_gyro_logic(void) {
    disp.writeCenter("Zeroing Gyro...",0);
    disp.writeDisplay();
	cout << "gyroAverage is " << gyroAverageZ << endl;
	if (stateLoopCount < 50) {
		gyroAverageZ += sensorData->gyroZ;
		gyroAverageY += sensorData->gyroY;
		gyroAverageX += sensorData->gyroX;
		stateLoopCount++;
	}
	else {
		gyroAverageZ /= (float)(stateLoopCount);
		gyroAverageY /= (float)(stateLoopCount);
		gyroAverageX /= (float)(stateLoopCount);
		angle_controller.setAverage(gyroAverageZ);
		position_tracker.setGyroAverages(gyroAverageX,gyroAverageY,gyroAverageZ);
		stateLoopCount = 0;
		currentState = nextState;
		cout << "measuring took " << timer.getTimeElapsed(PRECISION_MS) << " ms.\n";
		cout << "average is " << gyroAverageZ << endl;
	}
    disp.clearDisplay();
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
				setDriveDirection(STRAFE_LEFT,4.0);
				inner_state++;
			}
			break;
		case 2:
			if ((sensorData->ir1_1_state == 0) && (sensorData->ir1_2_state == 0)) {
				setDriveDirection(STRAFE_RIGHT,3.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 3:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 300) {
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
				voltage_max = 3.5;
			}
			else if (translation_angle < -95.0) {
				translation_angle = -75.0;
				voltage_max = 3.5;
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
	switch(inner_state) {
		case 0:
			state_timer.start();
			inner_state++;
			break;
		case 1:
			setDriveDirection(STRAFE_RIGHT,3.0);
			if((sensorData->ir1_1_state == 1) && (sensorData->ir1_2_state == 1)) {
				setDriveDirection(STOPPED,0.0);
				inner_state++;
			}
			break;
		case 2:
			setDriveDirection(STRAIGHT_FORWARD,3.5);
			inner_state++;
			break;
		case 3:
			if((sensorData->ir2_1_state == 0) && (sensorData->ir2_2_state == 0)) {
				setDriveDirection(STRAFE_LEFT,3.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 4:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				setDriveDirection(STRAFE_LEFT,4.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 5:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				setDriveDirection(STOPPED,0.0);
				inner_state = 0;
				nextState = pre_stage2;
				currentState = zero_gyro;
			}
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
			if (sensorData->ir2_4_state == 0) {
				setDriveDirection(STRAFE_RIGHT,3.0);
				inner_state++;
			}
			break;
		case 2:
			if (sensorData->ir2_3_state == 0) {
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
			if (state_timer.getTimeElapsed(PRECISION_MS) > 600) {
				setDriveDirection(STOPPED,0.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 5:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				inner_state = 0;
				currentState = average_compass;
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
	angle_controller.disableIntegral();
	if (stateLoopCount < 50) {
		compAverage += sensorData->compass_angle;
		stateLoopCount++;
	}
	else {
		compAverage /= 50.0;
		stateLoopCount = 0;
		cout << "average is " << compAverage << endl;
		currentState = stage2;
	}
}

void Robot::stage2_logic(void) {
	if (stateLoopCount == 0) {
		state_timer.start();
		stateLoopCount++;
	}

	if((sensorData->compass_angle - compAverage) > 10 || (sensorData->compass_angle - compAverage) < -10) {
		cout << "Stopping. Field Diff: " << sensorData->compass_angle - compAverage << endl;
		slapper_voltage = 0;
	}
	else {
		cout << "Slapping. Field Diff: " << sensorData->compass_angle - compAverage << endl;
		slapper_voltage = 4.5;
	}

	if (state_timer.getTimeElapsed(PRECISION_S) > 32) {
		slapper_voltage = 0;
		stateLoopCount = 0;
		inner_state = 0;
		currentState = post_stage2;
	}
}

void Robot::post_stage2_logic(void) {
	angle_controller.enableIntegral(IGAIN);
	switch(inner_state) {
		case 0:
			setDriveDirection(STRAIGHT_BACKWARD,3.0);
			inner_state++;
			break;
		case 1:
			if ((sensorData->ir2_4_state == 1) && (sensorData->ir2_3_state == 1))
			{
				angle_controller.setSetpoint(-90.0);
				setDriveDirection(ROTATE,3.0);
				inner_state++;
			}
			break;
		case 2:
			//setDriveDirection(STRAFE_RIGHT,4.5);
			if ((angle_controller.getAngle() < -85.0) && (angle_controller.getAngle() > -95.0)) {
				setDriveDirection(STRAIGHT_FORWARD,5.0);
				inner_state++;
			}
			break;
		case 3:
			if ((sensorData->ir2_1_state == 0) && (sensorData->ir2_2_state == 0))
			{
				setDriveDirection(STOPPED,0.0);
				slapper_voltage = 0.0;
				inner_state = 0;
				currentState = pre_stage3;
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

void Robot::pre_stage3_logic(void) {
	if (stateLoopCount == 0) {
		state_timer.start();
	}
	cout << "state timer is " << state_timer.getTimeElapsed(PRECISION_MS) << "ms\n";
	switch (inner_state) {
		case 0:
			setDriveDirection(STRAFE_RIGHT,3.5);
			if (state_timer.getTimeElapsed(PRECISION_S) > 1) {
				inner_state++;
			}
			break;
		case 1:
			if (sensorData->ir2_4_state == 0) {
				setDriveDirection(STRAFE_RIGHT,3.0);
				inner_state++;
			}
			break;
		case 2:
			if (sensorData->ir2_3_state == 0) {
				setDriveDirection(STRAIGHT_FORWARD,3.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 3:
			//if ((sensorData->ir2_1_state == 1) && (sensorData->ir2_2_state == 1)) {
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				setDriveDirection(STRAIGHT_BACKWARD,2.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 4:
			cout << state_timer.getTimeElapsed(PRECISION_MS) << endl;
			if (state_timer.getTimeElapsed(PRECISION_MS) > 50) {
				setDriveDirection(STOPPED,0.0);
				inner_state = 0;
				currentState = stage3;
				slapper_voltage = 0.0;
			}
			break;
	}
	drive_logic();
	stateLoopCount++;
}

void Robot::stage3_logic(void) {
	currentState = post_stage3;
}

void Robot::post_stage3_logic(void) {
	static float angleX_before_step = position_tracker.getAngle(X);
	cout << "difference is " << position_tracker.getAngle(X) - angleX_before_step << endl;
	switch(inner_state) {
		case 0:
			setDriveDirection(STRAIGHT_BACKWARD,3.0);
			inner_state++;
			break;
		case 1:
			if ((sensorData->ir2_4_state == 1) && (sensorData->ir2_3_state == 1))
			{
				angle_controller.setSetpoint(-180.0);
				setDriveDirection(ROTATE,3.0);
				inner_state++;
			}
			break;
		case 2:
			if ((angle_controller.getAngle() < -175.0) && (angle_controller.getAngle() > -185.0)) {
				setDriveDirection(STRAIGHT_FORWARD,4.0);
				state_timer.start();
				inner_state = 5;
			}
			break;
		/*	
		case 3:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				setDriveDirection(STRAFE_LEFT,4.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 4:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				setDriveDirection(STOPPED,0.0);
				stateLoopCount = 0;
				nextState = currentState;
				currentState = zero_gyro;
				angle_controller.disableIntegral();
				angle_controller.enableIntegral(IGAIN);
				inner_state++;
			}
			break;
		*/
		case 5:
			setDriveDirection(STRAFE_RIGHT,4.0);
			state_timer.start();
			inner_state++;
			break;
		case 6:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				setDriveDirection(STRAIGHT_FORWARD,4.5);
				inner_state++;
			}
			break;
		case 7:
			if ((position_tracker.getAngle(X) - angleX_before_step) < -0.1) {
				inner_state++;
			}
			break;
		case 8:
			if ((position_tracker.getAngle(X) - angleX_before_step) > 0.0) {
				inner_state++;
				voltage_max = 7.0;
			}
			break;
		case 9:
			if ((position_tracker.getAngle(X) - angleX_before_step) < -0.5) {
				//state_timer.start();
				inner_state++;
			}
			break;
		case 10:
			if ((position_tracker.getAngle(X) - angleX_before_step) > 0.0) {
				inner_state = 0;
				currentState = pre_stage4;
				//inner_state++;
				//voltage_max = 6.0;
			}
			/*else if (state_timer.getTimeElapsed(PRECISION_MS) > 500) {
				state_timer.start();
				voltage_max += 0.1;
			}*/
			break;
		/*
		case 7:
			if ((position_tracker.getAngle(X) - angleX_before_step) < -4.0) {
				state_timer.start();
				inner_state++;
			}
			break;
		case 8:
			if ((position_tracker.getAngle(X) - angleX_before_step) > -2.0) {
				inner_state = 0;
				currentState = pre_stage4;
			}
			//else if (state_timer.getTimeElapsed(PRECISION_MS) > 500) {
			//	state_timer.start();
			//	voltage_max += 0.1;
			//}
			break;
		*/

	}
	cout << "angle x is " << position_tracker.getAngle(X) << endl;
	drive_logic();
}

void Robot::pre_stage4_logic(void) {
	static float angleX_before_step = position_tracker.getAngle(X);
	cout << "difference is " << position_tracker.getAngle(X) - angleX_before_step << endl;
	switch (inner_state) {
		case 0:
			angle_controller.setSetpoint(-90.0);
			setDriveDirection(ROTATE,3.0);
			inner_state++;
			break;
		case 1:
			if ((angle_controller.getAngle() < -85.0) && (angle_controller.getAngle() > -95.0)) {
				setDriveDirection(STRAFE_RIGHT,4.0);
				//setDriveDirection(STRAIGHT_BACKWARD,8.4);
				state_timer.start();
				inner_state++;
			}
			break;
		//case 2:
		//	if ((position_tracker.getAngle(X) - angleX_before_step) > 5.0) {
		//		state_timer.start();
		//		setDriveDirection(STRAIGHT_BACKWARD,4.0);
		//		inner_state++;
		//	}
		//	/*else if (state_timer.getTimeElapsed(PRECISION_MS) > 500) {
		//		state_timer.start();
		//		voltage_max += 0.1;
		//	}*/
		//	break;
		//case 3:
		//	if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
		//		setDriveDirection(STOPPED,0.0);
		//		inner_state = 0;
		//		currentState = stage4;
		//	}
		//	break;

		case 2:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1500) {
				setDriveDirection(STOPPED,3.0);
				inner_state++;
				nextState = currentState;
				currentState = zero_gyro;
			}
			break;
		case 3:
			angle_controller.disableIntegral();
			angle_controller.enableIntegral(IGAIN);
			setDriveDirection(STRAFE_LEFT,3.0);
			state_timer.start();
			inner_state++;
			break;
		case 4:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 400) {
				setDriveDirection(STRAIGHT_FORWARD,3.0);
				inner_state++;
			}
			break;
		case 5:
			if ((sensorData->ir2_1_state == 0) && (sensorData->ir2_2_state == 0)){
				setDriveDirection(STOPPED, 0.0);

				camera_direction = MOVE_RIGHT;
				end_thread_flag = 0;
				pthread_mutex_init(&cam_direction_mutex, NULL);
				pthread_mutex_init(&end_thread_flag_mutex, NULL);

				camera_data data;
				data.direction_ptr = &camera_direction;
				data.end_thread_ptr = &end_thread_flag;
				data.dir_mutex_ptr = &cam_direction_mutex;
				data.end_mutex_ptr = &end_thread_flag_mutex;

				pthread_create(&camera_thread, NULL, locate, (void *)&data);

				inner_state++;
			}
			break;
		case 6:
			pthread_mutex_lock(&cam_direction_mutex);
			switch(camera_direction) {
				case MOVE_RIGHT:
					setDriveDirection(STRAIGHT_BACKWARD, 2.0);
					break;
				case MOVE_LEFT:
					setDriveDirection(STRAIGHT_FORWARD, 2.0);
					break;
				case STOP:
					setDriveDirection(STOPPED, 0.0);
					pthread_mutex_lock(&end_thread_flag_mutex);
					end_thread_flag = 1;
					pthread_mutex_unlock(&end_thread_flag_mutex);
					inner_state = 0;
					currentState = stage4;
					break;
			}
			pthread_mutex_unlock(&cam_direction_mutex);
			break;


		/*case 5:
			if ((sensorData->ir2_1_state == 0) && (sensorData->ir2_2_state == 0)) {
				setDriveDirection(STRAIGHT_BACKWARD,3.0);
				inner_state++;
			}
			break;
		case 6:
			if (sensorData->ir4_2_state == 0) {
				setDriveDirection(STRAIGHT_BACKWARD,2.0);
				inner_state++;
			}
			break;
		case 7:
			if (sensorData->ir4_1_state == 0) {
				setDriveDirection(STRAFE_RIGHT,3.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 8:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				setDriveDirection(STRAIGHT_FORWARD,2.0);
				setDriveDirection(STOPPED,0.0);
				inner_state++;
				inner_state = 0;
				currentState = stage4;
			}
			break;
		case 9:
			if (sensorData->ir4_1_state == 1) {
				setDriveDirection(STRAIGHT_BACKWARD,2.0);
				inner_state++;
			}
			break;
		case 10:
			if (sensorData->ir4_1_state == 0) {
				setDriveDirection(STOPPED,0.0);
				inner_state = 0;
				currentState = stage4;
			}
			break;*/

	}
	drive_logic();
}

void Robot::stage4_logic(void) {
	switch (inner_state) {
		case 0:
			state_timer.start();
			setDirectionGPIO(GUN_GPIO,GPIO_OUTPUT);
			writeGPIO(GUN_GPIO,1);
			inner_state++;
			break;
		case 1:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				writeGPIO(GUN_GPIO,0);
				setDirectionGPIO(GUN_GPIO,GPIO_INPUT);
				inner_state = 0;
				currentState = finish;
				cout << "\nFIRE\n\n";
			}
			break;
	}
}

void Robot::finish_logic(void) {
	cout << "Stage 1 code is " <<
		stage1.components[0] << ' ' <<
		stage1.components[1] << ' ' <<
		stage1.components[2] << ' ' <<
		stage1.components[3] << ' ' <<
		stage1.components[4] << endl;
}
