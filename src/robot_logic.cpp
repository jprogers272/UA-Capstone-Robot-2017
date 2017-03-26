//Author(s): John Rogers, William Khan, Brandon Fikes

#include "robot.hpp"
#include "gpio.hpp"
#include "mecanum.hpp"
#include "positionTracker.hpp"
#include "sensors.hpp"
#include "stage1.hpp"
#include "timing.hpp"
#include "stage3.hpp"

#include <iostream>
#include <sstream>

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
    if(display_flag == 0) {
	   disp.writeCenter("Robot Starting Up...",0);
	   disp.writeDisplay();
	   display_flag = 1; 
    }

    currentState = zero_gyro;
	stateLoopCount = 0;
	timer.start();
    disp.clearDisplay();
    display_flag = 0;
}

void Robot::zero_gyro_logic(void) {
  	if(display_flag == 0) {
		disp.writeCenter("Zeroing Gyro...",0);
		disp.writeDisplay();
		display_flag = 1;
	}
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
		disp.clearDisplay();
		display_flag = 0;
		cout << "measuring took " << timer.getTimeElapsed(PRECISION_MS) << " ms.\n";
		cout << "average is " << gyroAverageZ << endl;
	}
}

void Robot::pre_stage1_logic(void) {
	cout << "inner state is " << inner_state << endl;
	if(display_flag == 0) {	
		disp.writeCenter("Prepping Stage 1...",0);
		disp.writeDisplay();
		display_flag = 1;
	}
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
			if (state_timer.getTimeElapsed(PRECISION_MS) > 200) {
				setDriveDirection(STRAIGHT_FORWARD,3.0);
				inner_state++;
			}
			break;
		case 4:
			if (sensorData->ir1_3_state == 0) {
				setDriveDirection(STRAIGHT_FORWARD,2.0);
				inner_state++;
				state_timer.start();
			}
			break;
		case 5:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 300) {
				inner_state++;
			}
			break;
		case 6:
			if ((sensorData->ir1_4_state == 1)&&(sensorData->ir1_3_state==0)) {
				setDriveDirection(STRAIGHT_BACKWARD,2.0);
				inner_state++;
			}
			break;
		case 7:
			if ((sensorData->ir1_3_state == 0)&&(sensorData->ir1_4_state==0)) {
				setDriveDirection(STRAFE_LEFT,2.5);
				inner_state = 0;
				currentState = stage1_solving;
				display_flag = 0;
			}
			break;	

	/*	case 5:
			if ((sensorData->ir1_3_state == 1)&&(sensorData->ir1_4_state==0)) {
				setDriveDirection(STRAFE_LEFT,2.5);
				inner_state = 0;
				currentState = stage1_solving;
				display_flag = 0;
			}
			break;*/
	}
	drive_logic();
}

void Robot::stage1_logic(void) {
	switch (inner_state) {
		case 0:
			if(display_flag == 0) {
				disp.writeCenter("Solving Stage 1...",1);
				disp.writeDisplay();
				display_flag =1;
			}
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
					display_flag = 0;
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
				translation_angle = -55.0;
				voltage_max = 3.5;
			}
			else {
				//it's 90.0, accounting for floating point error
				translation_angle = -55.0;
			}
		       	state_timer.start();
			inner_state = 4;
			stage1.currentComponent = 0;
			break;
		case 4:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				inner_state = 0;
			}
			break;
	}
	drive_logic();
}

void Robot::post_stage1_logic(void) {
	ostringstream s; 
	cout << "code is "
		<< stage1.components[0] << ' '
		<< stage1.components[1] << ' '
		<< stage1.components[2] << ' '
		<< stage1.components[3] << ' '
		<< stage1.components[4] << endl;
	s << "code is " << stage1.components[0] << ' '
		<< stage1.components[1] << ' '
		<< stage1.components[2] << ' '
		<< stage1.components[3] << ' '
		<< stage1.components[4];
	if(display_flag == 0) {
		disp.writeCenter(s.str(), 7);
		disp.writeDisplay();
		display_flag = 1;
	}
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
				display_flag = 0;
			}
	}
	drive_logic();
}

void Robot::pre_stage2_logic(void) {
	cout << "inner state is " << inner_state << endl;

	if(display_flag == 0) {	
		disp.writeCenter("Prepping Stage 2...",2);
		disp.writeDisplay();
		display_flag = 1;
	}
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
				display_flag = 0;
			}
			break;
		default:
			setDriveDirection(STOPPED,0.0);
			inner_state = 0;
			currentState = average_compass;
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
	if(display_flag == 0) {
		disp.writeCenter("Completing Stage 2...",2);
		disp.writeDisplay();
		display_flag = 1;
	}
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
		display_flag = 0;
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
	// disp.writeCenter("Solving Stage 3...",3); // Once logic is in place, place inside if (display_flag == 0)
	// disp.writeDisplay();
	switch(inner_state){
		case 0: //configure pins used, setup variables
			setDirectionGPIO(STEP, 0); //STEP  = GPIO PIN 45 (OUTPUT)
			setDirectionGPIO(STEPDIRECTION, 0); //DIR   = GPIO PIN 44 (OUTPUT)
			setDirectionGPIO(ENABLEDRIVER, 0); //SLEEP PIN used for Enable = GPIO PIN 46 (OUTPUT)
			writeGPIO(ENABLEDRIVER, 1); //wake the driver from sleep
			writeGPIO(STEPDIRECTION, 1); //set rotation direction to clockwise
			stage3.waiting = 0;
			stage3.calculateRotations(stage1.components*);
			inner_state++;
			break;
		case 1: //First Rotation
			//Check to see if loop variable has not expired, and if the robot is not in a wait state
			if (stage3.rotate1 != 0 && stage3.waiting != 1){
				writeGPIO(STEP, 1); //Begin Pulse
				state_timer.start(); //start timer for pulse
				stage3.waiting = 1; //Indicate that Stage 3 is waiting to complete the pulse
			}
			//Check to see if we are waiting to complete a step - wait time is 10 milliseconds
			if ((stage3.waiting == 1) && (state_timer.getTimeElapsed(PRECISION_MS) >= 10)){
				writeGPIO(STEP, 0); //Complete Pulse
				stage3.waiting = 0; //clear wait variable
				--stage3.rotate1;   //decrement loop variable used to track rotations
			}
			//Check to see if loop variable has expired
			if (stage3.rotate1 == 0){
				writeGPIO(STEPDIRECTION, 0); //set rotation direction to counter-clockwise
				stage3.waiting = 0; //ensure wait is cleared
				inner_state++; //move to second rotation state
			}
			break;
		case 2: //Second Rotation
			if (stage3.rotate2 != 0 && stage3.waiting != 1){
				writeGPIO(STEP, 1); //Begin Pulse
				state_timer.start(); //start timer for pulse
				stage3.waiting = 1; //Indicate that Stage 3 is waiting to complete the pulse
			}
			if ((stage3.waiting == 1) && (state_timer.getTimeElapsed(PRECISION_MS) >= 10)){
				writeGPIO(STEP, 0); //Complete Pulse
				stage3.waiting = 0; 
				--stage3.rotate2; 
			}
			if (stage3.rotate2 == 0){
				writeGPIO(STEPDIRECTION, 1); //set rotation direction to clockwise
				stage3.waiting = 0; //ensure wait is cleared
				inner_state++;
			}
			break;
		case 3: //Third Rotation
			if (stage3.rotate3 != 0 && stage3.waiting != 1){
				writeGPIO(STEP, 1); //Begin Pulse
				state_timer.start(); //start timer for pulse
				stage3.waiting = 1; //Indicate that Stage 3 is waiting to complete the pulse
			}
			if ((stage3.waiting == 1) && (state_timer.getTimeElapsed(PRECISION_MS) >= 10)){
				writeGPIO(STEP, 0); //Complete Pulse
				stage3.waiting = 0;
				--stage3.rotate3;
			}
			if (stage3.rotate3 == 0){
				writeGPIO(STEPDIRECTION, 0); //set rotation direction to counter-clockwise
				stage3.waiting = 0; //ensure wait is cleared
				inner_state++;
			}
			break;
		case 4: //Fourth Rotation
			if (stage3.rotate4 != 0 && stage3.waiting != 1){
				writeGPIO(STEP, 1); //Begin Pulse
				state_timer.start(); //start timer for pulse
				stage3.waiting = 1; //Indicate that Stage 3 is waiting to complete the pulse
			}
			if ((stage3.waiting == 1) && (state_timer.getTimeElapsed(PRECISION_MS) >= 10)){
				writeGPIO(STEP, 0); //Complete Pulse
				stage3.waiting = 0;
				--stage3.rotate4;
			}
			if (stage3.rotate4 == 0){
				writeGPIO(STEPDIRECTION, 1); //set rotation direction to clockwise
				stage3.waiting = 0; //ensure wait is cleared
				inner_state++;
			}
			break;
		case 5: //Fifth Rotation
			if (stage3.rotate5 != 0 && stage3.waiting != 1){
				writeGPIO(STEP, 1); //Begin Pulse
				state_timer.start(); //start timer for pulse
				stage3.waiting = 1; //Indicate that Stage 3 is waiting to complete the pulse
			}
			if ((stage3.waiting == 1) && (state_timer.getTimeElapsed(PRECISION_MS) >= 10)){
				writeGPIO(STEP, 0); //Complete Pulse
				stage3.waiting = 0;
				--stage3.rotate5;
			}
			if (stage3.rotate5 == 0){
				stage3.waiting = 0; //ensure wait is cleared
				inner_state++;
			}
			break;
		case 6: //Clean Up
			writeGPIO(ENABLEDRIVER, 0); //put driver to sleep and release stepper
			inner_state = 0;
			currentState = post_stage3;
	}
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
				currentState = finish;
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
	if(display_flag == 0) {
		disp.writeCenter("Prepping Stage 4...",4);
		disp.writeDisplay();
		display_flag = 1;
	}
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
				display_flag = 0;
			}
			break;

	}
	drive_logic();
}

void Robot::stage4_logic(void) {
	switch (inner_state) {
		case 0:
			if(display_flag == 0) {
				disp.writeCenter("Completing Stage 4...",5);
				disp.writeDisplay();
				display_flag = 1;
			}
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
				display_flag = 0;
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

		disp.writeCenter("Done Stage 4...",6);
		disp.writeDisplay();
}
