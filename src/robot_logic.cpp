//Author(s): John Rogers, William Khan, Brandon Fikes, Bryant Hall, Tyler Henson, Dakota Turner

#include "robot.hpp"
#include "gpio.hpp"
#include "mecanum.hpp"
#include "positionTracker.hpp"
#include "sensors.hpp"
#include "stage1.hpp"
#include "timing.hpp"
#include "stage3.hpp"
#include "camera.hpp"

#include <iostream>
#include <sstream>

using namespace std;

void Robot::drive_logic(void) {
	position_tracker.calculateAnglesOnly(sensorData->gyroX,sensorData->gyroY,sensorData->gyroZ,timer.getTimeElapsed(PRECISION_MS));
	rotation = pid_multiplier * angle_controller.calculateRotation(sensorData->gyroZ,timer.getTimeElapsed(PRECISION_MS));
	processMecanum(drive_voltages,voltage_max,translation,translation_angle,rotation*3.0/voltage_max);
//	cout << "drive voltages are " << drive_voltages[0] << ", " <<
//		drive_voltages[1] << ", " <<
//		drive_voltages[2] << ", " <<
//		drive_voltages[3] << endl;
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
	if (start_switch_flag == 0) {
		if (readGPIO(START_SWITCH_GPIO) == 1) {
			resetRobot();
			start_switch_flag = 1;
		}
	}
	else {
		if (readGPIO(START_SWITCH_GPIO) == 0) {
			currentState = zero_gyro;
			timer.start();
			disp.clearDisplay();
			display_flag = 0;
			start_switch_flag = 0;
		}
	}
}

void Robot::zero_gyro_logic(void) {
	state_timer.start();
  	if(display_flag == 0) {
		disp.writeCenter("Zeroing Gyro...",0);
		disp.writeDisplay();
		display_flag = 1;
		gyroAverageX = 0.0;
		gyroAverageY = 0.0;
		gyroAverageZ = 0.0;
		angle_controller.disableIntegral();
	}
//	cout << "gyroAverage is " << gyroAverageZ << endl;
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
		//angle_controller.setSetpoint(0.0);
		//angle_controller.setAngle(angle_controller.getSetpoint());
		angle_controller.enableIntegral(IGAIN);
		angle_controller.setTime(timer.getTimeElapsed(PRECISION_MS));
		position_tracker.setGyroAverages(gyroAverageX,gyroAverageY,gyroAverageZ);
		stateLoopCount = 0;
		currentState = nextState;
		disp.clearDisplay();
		display_flag = 0;
		cout << "measuring took " << state_timer.getTimeElapsed(PRECISION_MS) << " ms.\n";
		cout << "average is " << gyroAverageZ << endl;
	}
}

void Robot::pre_stage1_logic(void) {
//	cout << "inner state is " << inner_state << endl;
	cout << "pre stage 1 and " << inner_state << endl;
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
				setDriveDirection(STRAFE_LEFT,5.0);
				state_timer.start();
				inner_state++;
			}
			break;	
		case 8:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1500) {
				inner_state = 0;
				currentState = stage1_solving;
				display_flag = 0;
				state_timer2.start();
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
	//cout << "stage 1 and " << inner_state << endl;
	if (state_timer2.getTimeElapsed(PRECISION_MS) > 10000) {
		state_timer.start();
		setDriveDirection(STRAFE_RIGHT,6.0);
		inner_state = 7;
		stage1.deEnergizeComponent();
		stage1.deEnergizeCommon();
		stage1.currentComponent = 0;
	}
	switch (inner_state) {
		case 0:
			if(display_flag == 0) {
				disp.writeCenter("Solving Stage 1...",1);
				disp.writeDisplay();
				display_flag = 1;
			}
			//stage1.energizeComponent();
			stage1.energizeCommon();
			state_timer.start();
			inner_state++;
			break;
		case 1:
			if (state_timer.getTimeElapsed(PRECISION_MS) > STAGE1_CHARGING_TIME) {
				stage1.checkReverse(STAGE1_IGNORE_PIN);
				stage1.deEnergizeCommon();
				stage1.energizeComponent();
				state_timer.start();
				inner_state++;
			}
			break;
		case 2:
			//cout << "timer value is " << state_timer.getTimeElapsed(PRECISION_MS) << "ms\n";
			//cout << "current component is " << stage1.currentComponent << ", " << stage1.components[stage1.currentComponent] << "\n";
			if (stage1.currentComponent == STAGE1_IGNORE_PIN) {
				stage1.deEnergizeComponent();
				stage1.currentComponent++;
				stage1.energizeComponent();
				state_timer.start();
				//inner_state = 0;
			}
			else if (stage1.currentComponent < 5) {
				if (state_timer.getTimeElapsed(PRECISION_MS) > STAGE1_CHARGING_TIME) {
					stage1.identifyComponent();
					cout << "identified component " << stage1.currentComponent << " as " << stage1.components[stage1.currentComponent] << endl;
					stage1.deEnergizeComponent();
					stage1.currentComponent++;
					stage1.energizeComponent();
					state_timer.start();
					//inner_state = 0;
				}
			}
			else {
				//stage1.energizeCommon();
				//stage1.deEnergizeComponent();
				state_timer.start();
				inner_state++;
			}
			break;
		case 3:
			if (state_timer.getTimeElapsed(PRECISION_MS) > STAGE1_CHARGING_TIME) {
				//stage1.checkCapacitorDiode(STAGE1_IGNORE_PIN);
				//deduce pin that was untested. 1+2+3+4+5=15, and components[STAGE1_IGNORE_PIN] is initially zero
				//cout << stage1.components[0] << stage1.components[1] << stage1.components[2] << stage1.components[3] << stage1.components[4] << endl;
				stage1.components[STAGE1_IGNORE_PIN] = 15 - stage1.components[0] - 
					stage1.components[1] - stage1.components[2] - 
					stage1.components[3] - stage1.components[4];
				cout << stage1.components[0] << stage1.components[1] << stage1.components[2] << stage1.components[3] << stage1.components[4] << endl;
				if (stage1.detectProblems() == 0) {
					//setDriveDirection(STOPPED,0);
					inner_state = 0;
					currentState = post_stage1;
					display_flag = 0;
				}
				else {
					inner_state++;
				}
			}
			break;
		case 4:
			cout << "reorient robot\n";
			stage1.zeroComponentArray();
			voltage_max = 2.0;
			if (translation_angle > -89.0) {
				translation_angle = -180.0;
			}
			else if ((translation_angle < -91.0) && (translation_angle > -95)) {
				translation_angle = 0.0;
			}
			else {
				//it's 90.0, accounting for floating point error
				translation_angle = 0.0;
			}
		    state_timer.start();
			inner_state++;
			stage1.currentComponent = 0;
			break;
		case 5:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 500) {
				voltage_max = 5.0;
				if (translation_angle > -90.0) {
					translation_angle = -88.0;
				}
				else {
					translation_angle = -92.0;
				}
				state_timer.start();
				inner_state++;
			}
			break;
		case 6:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				inner_state = 0;
				state_timer.start();
			}
			break;
		case 7:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 300) {
				state_timer.start();
				inner_state++;
				//currentState = pre_stage1;
				setDriveDirection(STRAIGHT_BACKWARD,6.0);
			}
			break;
		case 8:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 300) {
				state_timer.start();
				inner_state = 2;
				currentState = pre_stage1;
				setDriveDirection(STRAFE_LEFT,4.0);
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
			if (state_timer.getTimeElapsed(PRECISION_S) > 1) {
				state_timer.start();
				inner_state++;
			}
			break;
		case 1:
			cout << state_timer.getTimeElapsed(PRECISION_MS) << endl;
			if (state_timer.getTimeElapsed(PRECISION_MS)>2500)
			{ 
				if (sensorData->ir2_1_state == 0 && sensorData->ir2_2_state == 0)
				{
					setDriveDirection(STRAIGHT_BACKWARD,3.0);
					inner_state++;
				}
				else
				{
					setDriveDirection(STRAIGHT_FORWARD,3.0);  
					state_timer.start();
					inner_state = 3;
				}
			}
			break;
		case 2:  
			if (sensorData->ir2_1_state == 1 && sensorData->ir2_2_state == 1)
				{
					setDriveDirection(STRAIGHT_FORWARD,3.0);
					inner_state++;
				}
			break;
		case 3:  
		if (sensorData->ir2_1_state == 0 && sensorData->ir2_2_state == 0)
			{
				setDriveDirection(STRAIGHT_FORWARD,2.0);
				state_timer.start();
				inner_state++;
			}
		break;
		case 4: 
			if ((state_timer.getTimeElapsed(PRECISION_MS) > 200))
			{
				setDriveDirection(STRAFE_LEFT,3.0);
				inner_state++;
			}
			break;
		case 5:
			if (sensorData->ir2_3_state == 0 && sensorData->ir2_4_state == 0){
				setDriveDirection(STRAIGHT_FORWARD,2.0);
				state_timer.start();
				inner_state++;
			}
			break;

		case 6:
			cout << state_timer.getTimeElapsed(PRECISION_MS) << endl;
			if (state_timer.getTimeElapsed(PRECISION_MS) > 500) {
				setDriveDirection(STOPPED,0.0);
				state_timer.start();
				inner_state++;	
			}
			break;
		case 7:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				inner_state = 0;
				stateLoopCount = 0;
				currentState = average_compass;
			}
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
		slapper_voltage = 6.0;
	}

	if (state_timer.getTimeElapsed(PRECISION_S) > 2) {
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
				setDriveDirection(STRAIGHT_FORWARD,4.0);
				inner_state++;
			}
			break;
		case 3:
			if ((sensorData->ir2_1_state == 0) && (sensorData->ir2_2_state == 0))
			{
				setDriveDirection(STRAIGHT_BACKWARD,3.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 4:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 500){
				setDriveDirection(STRAFE_LEFT, 3.0);
				state_timer.start();
				inner_state++;
			}

		case 5:
			if (state_timer.getTimeElapsed(PRECISION_MS)>1000)
			{
				setDriveDirection(STRAFE_LEFT,4.0);
				slapper_voltage = 0;
				inner_state++;
				state_timer.start();
			}
			break;
		case 6: 
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000){
				setDriveDirection(STOPPED, 0.0);
				inner_state = 0;
				currentState = zero_gyro;
				nextState = pre_stage3;
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
	//cout << "state timer is " << state_timer.getTimeElapsed(PRECISION_MS) << "ms\n";
	switch (inner_state) {
		case 0:
			setDriveDirection(STRAFE_RIGHT,3.5);
			if (state_timer.getTimeElapsed(PRECISION_S) > 1) {
				state_timer.start();
				inner_state++;
			}
		break;

		case 1:
			cout << state_timer.getTimeElapsed(PRECISION_MS) << endl;
			if (state_timer.getTimeElapsed(PRECISION_MS)>3500)
			{ 
				if (sensorData->ir2_1_state == 0 && sensorData->ir2_2_state == 0)
				{
					setDriveDirection(STRAIGHT_BACKWARD,3.0);
					inner_state++;
				}
				else
				{
					setDriveDirection(STRAIGHT_FORWARD,3.0);  
					state_timer.start();
					inner_state = 3;
				}
			}
		break;

		case 2:  
			if (sensorData->ir2_1_state == 1 && sensorData->ir2_2_state == 1)
				{
					setDriveDirection(STRAIGHT_FORWARD,2.0);
					inner_state++;
				}
		break;

		case 3:  
			if (sensorData->ir2_1_state == 0 && sensorData->ir2_2_state == 0)
				{
					setDriveDirection(STRAIGHT_FORWARD,2.0);
					state_timer.start();
					inner_state++;
				}
		break;

		case 4: 
			if ((state_timer.getTimeElapsed(PRECISION_MS) > 100))
			{
				setDriveDirection(STRAFE_LEFT,3.0);
				inner_state++;
			}
			break;

		case 5:
			if (sensorData->ir2_3_state == 0 && sensorData->ir2_4_state == 0){
				setDriveDirection(STRAIGHT_FORWARD,1.5);
				state_timer.start();
				inner_state++;
			}
			break;

		case 6:
			cout << state_timer.getTimeElapsed(PRECISION_MS) << endl;
			if (state_timer.getTimeElapsed(PRECISION_MS) > 2000) {
				setDriveDirection(STOPPED,0.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 7:
			if (state_timer.getTimeElapsed(PRECISION_MS)>1000){
				setDriveDirection(STRAIGHT_BACKWARD,1.5);
				state_timer.start();
				inner_state++;
			}
			break;
		case 8:
			if (state_timer.getTimeElapsed(PRECISION_MS)>50)
			{
				setDriveDirection(STOPPED,0.0);
				state_timer.start();
				inner_state++;
			}
			break;
		case 9:
			if (state_timer.getTimeElapsed(PRECISION_MS)>500)
			{
				inner_state = 0;
				currentState = stage3_solving;
			}	
			break;

	}
	drive_logic();
	stateLoopCount++;
}

void Robot::stage3_logic(void) {
	
	//int loopvar = 0;

	//robotWait(0,500);
	cout << "inner state is " << inner_state << endl;
	switch(inner_state){
		case 0: //configkkure pins used, setup variables
			//disp.writeCenter("Solving Stage 3...",3); // Once logic is in place, place inside if (display_flag == 0)
			//disp.writeDisplay();
			setDirectionGPIO(STEP, 1);
			setDirectionGPIO(STEPDIRECTION, 0); //DIR   = GPIO PIN 44 (OUTPUT)
			setDirectionGPIO(ENABLEDRIVER, 0); //SLEEP PIN used for Enable = GPIO PIN 46 (OUTPUT)
			setDirectionGPIO(STEP_RUN, 0); //used to start process
			setDirectionGPIO(PROCESS_UPDATE, 1); //used for input from arduino
			writeGPIO(ENABLEDRIVER, 1); //wake the driver from sleep
			writeGPIO(STEPDIRECTION, 1); //set rotation direction to clockwise
			stage3.waiting = 0;
			stage3.calculateRotations(stage1.components);
			//stage1.components[0] = 3;
			//stage1.components[1] = 4;
			//stage1.components[2] = 2;
			//stage1.components[3] = 1;
			//stage1.components[4] = 5;
			//stage3.currentCodeValue = 0;
			inner_state++;
			break;

		case 1:
			writeGPIO(STEP_RUN,1);
			state_timer.start();
			inner_state++;
			break;
		case 2:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				writeGPIO(STEP_RUN,0);
				stage1.components[stage3.currentCodeValue]--;
				if (stage1.components[stage3.currentCodeValue] != 0) {
					inner_state = 1;
				}
				else {
					inner_state = 3;
					stage3.currentCodeValue++;
				}
			}
			break;
		case 3:
			if (stage3.currentCodeValue > 4) {
				currentState = post_stage3;
				inner_state = 0;
				writeGPIO(ENABLEDRIVER,0);
			}
			else {
				writeGPIO(STEPDIRECTION,!readGPIO(STEPDIRECTION));
				inner_state = 1;
			}
			break;
		/*
		case 1: //First Rotation
			if (stage3.rotate1 != 0 && stage3.waiting != 1){
				cout << "started first case" << endl;
				cout << "rotate 1, code number is " << stage3.rotate1 << endl;
				writeGPIO(STEP_RUN, 1); //start signal high
				cout << "wrote STEP_RUN" << endl;
				stage3.waiting = 1; //in wait state
				state_timer.start();
			}
			if (state_timer.getTimeElapsed(PRECISION_MS) > 2000) {
			//else if (readGPIO(PROCESS_UPDATE) == 1 && stage3.waiting == 1){ //done from arduino
				cout << "Process Update written High" << endl;
				writeGPIO(STEP_RUN, 0); //start signal low
				--stage3.rotate1; //update loop variable
				stage3.waiting = 0;
			}
			else if (stage3.rotate1 == 0){ //finish case
				cout << "In final case" << endl;
				writeGPIO(STEPDIRECTION, 0); //set rotation direction for next state
				stage3.waiting = 0;
				inner_state++;
				cout << "inner state changed, moving to state 2" << endl;
			
			}
			break;
		case 2: //Second Rotation
			if (stage3.rotate2 != 0 && stage3.waiting != 1){
				writeGPIO(STEP_RUN, 1); //start signal high
				stage3.waiting = 1; //in wait state
			}
			else if (readGPIO(PROCESS_UPDATE) == 1 && stage3.waiting == 1){ //done from arduino
				writeGPIO(STEP_RUN, 0); //start signal low
				--stage3.rotate2; //update loop variable
				stage3.waiting = 0;
			}
			else if (stage3.rotate2 == 0){ //finish case
				writeGPIO(STEPDIRECTION, 1); //set rotation direction for next state
				stage3.waiting = 0;
				inner_state++;
			}
			break;
		case 3: //Third Rotation
			if (stage3.rotate3 != 0 && stage3.waiting != 1){
				writeGPIO(STEP_RUN, 1); //start signal high
				stage3.waiting = 1; //in wait state
			}
			else if (readGPIO(PROCESS_UPDATE) == 1 && stage3.waiting == 1){ //done from arduino
				writeGPIO(STEP_RUN, 0); //start signal low
				--stage3.rotate3; //update loop variable
				stage3.waiting = 0;
			}
			else if (stage3.rotate3 == 0){ //finish case
				writeGPIO(STEPDIRECTION, 0); //set rotation direction for next state
				stage3.waiting = 0;
				inner_state++;
			}
			break;
		case 4: //Fourth Rotation
			if (stage3.rotate4 != 0 && stage3.waiting != 1){
				writeGPIO(STEP_RUN, 1); //start signal high
				stage3.waiting = 1; //in wait state
			}
			else if (readGPIO(PROCESS_UPDATE) == 1 && stage3.waiting == 1){ //done from arduino
				writeGPIO(STEP_RUN, 0); //start signal low
				--stage3.rotate4; //update loop variable
				stage3.waiting = 0;
			}
			else if (stage3.rotate4 == 0){ //finish case
				writeGPIO(STEPDIRECTION, 1); //set rotation direction for next state
				stage3.waiting = 0;
				inner_state++;
			}
			break;
		case 5: //Fifth Rotation
			if (stage3.rotate5 != 0 && stage3.waiting != 1){
				writeGPIO(STEP_RUN, 1); //start signal high
				stage3.waiting = 1; //in wait state
			}
			else if (readGPIO(PROCESS_UPDATE) == 1 && stage3.waiting == 1){ //done from arduino
				writeGPIO(STEP_RUN, 0); //start signal low
				--stage3.rotate5; //update loop variable
				stage3.waiting = 0;
			}
			else if (stage3.rotate5 == 0){ //finish case
				writeGPIO(STEPDIRECTION, 0); //set rotation direction for next state
				stage3.waiting = 0;
				inner_state++;
			}
			break;
		case 6: //Clean Up
			writeGPIO(ENABLEDRIVER, 0); //put driver to sleep and release stepper
			inner_state = 0;
			currentState = post_stage3;
			//break;
		*/
	}
	drive_logic();
}

void Robot::post_stage3_logic(void) {
//	static float angleX_before_step = position_tracker.getAngle(X_DIR);
//	cout << "difference is " << position_tracker.getAngle(X_DIR) - angleX_before_step << endl;
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
				//setDriveDirection(STRAIGHT_FORWARD,4.0);
				setDriveDirection(STRAFE_RIGHT,4.0);
				state_timer.start();
				inner_state = 6;
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
			if (state_timer.getTimeElapsed(PRECISION_MS) > 300) {
				setDriveDirection(STRAIGHT_FORWARD,4.5);
				state_timer.start();
				inner_state++;
			}
			break;
		case 7:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 2500) {
				//setDriveDirection(STRAIGHT_FORWARD,7.0);
				//state_timer.start();
				inner_state = 0;
				currentState = pre_stage4;
			}
			break;
		/*	
		case 8:
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1000) {
				inner_state = 0;
				currentState = pre_stage4;
			}
			break;
		
		case 7:
			if ((position_tracker.getAngle(X_DIR) - angleX_before_step) < -0.5) {
				inner_state++;
			}
			break;
		case 8:
			if ((position_tracker.getAngle(X_DIR) - angleX_before_step) > -0.5) {
				inner_state++;
				voltage_max = 7.0;
			}
			break;
		case 9:
			if ((position_tracker.getAngle(X_DIR) - angleX_before_step) < -0.6) {
				//state_timer.start();
				inner_state++;
			}
			break;
		case 10:
			if ((position_tracker.getAngle(X_DIR) - angleX_before_step) > -0.6) {
				inner_state = 0;
				currentState = pre_stage4;
				//inner_state++;
				//voltage_max = 6.0;
			}
			break;
		*/
			/*else if (state_timer.getTimeElapsed(PRECISION_MS) > 500) {
				state_timer.start();
				voltage_max += 0.1;
			}
			break;
			*/
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
//	cout << "angle x is " << position_tracker.getAngle(X) << endl;
	drive_logic();
}

void Robot::pre_stage4_logic(void) {
//	static float angleX_before_step = position_tracker.getAngle(X_DIR);
//	cout << "difference is " << position_tracker.getAngle(X) - angleX_before_step << endl;
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
			if (state_timer.getTimeElapsed(PRECISION_MS) > 1200) {
				setDriveDirection(STOPPED,0.0);
				inner_state++;
				stateLoopCount = 0;
				nextState = currentState;
				currentState = zero_gyro;
				angle_controller.setAngle(-90.0);
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
				//setDriveDirection(STRAIGHT_FORWARD,3.0);
				inner_state++;
			}
			break;
		case 5:
			//if ((sensorData->ir2_1_state == 0) && (sensorData->ir2_2_state == 0)){
				setDriveDirection(STOPPED, 0.0);

				camera_direction = MOVE_RIGHT;
				end_thread_flag = 0;
				processed_frame = 0;
				pthread_mutex_init(&cam_direction_mutex, NULL);
				pthread_mutex_init(&end_thread_flag_mutex, NULL);

				data.direction_ptr = &camera_direction;
				data.end_thread_ptr = &end_thread_flag;
				data.dir_mutex_ptr = &cam_direction_mutex;
				data.end_mutex_ptr = &end_thread_flag_mutex;
				data.processed_frame_ptr = &processed_frame;
				
				pthread_create(&camera_thread, NULL, locate, (void *)&data);
				inner_state++;
			//}
			break;
		case 6:
			pthread_mutex_lock(&cam_direction_mutex);
			if(processed_frame){
				switch(camera_direction) {
					case MOVE_RIGHT:
						setDriveDirection(STRAIGHT_BACKWARD, 1.5);
						inner_state++;
						state_timer.start();
						break;
					case MOVE_LEFT:
						setDriveDirection(STRAIGHT_FORWARD, 1.5);
						inner_state++;
						state_timer.start();
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
			}
			cout << camera_direction << endl;
			pthread_mutex_unlock(&cam_direction_mutex);
			break;

		case 7:
			if(state_timer.getTimeElapsed(PRECISION_MS) > 200){
				setDriveDirection(STOPPED, 0.0);
				pthread_mutex_lock(&cam_direction_mutex);
				processed_frame = 0;
				pthread_mutex_unlock(&cam_direction_mutex);
				inner_state--;
			}




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
				inner_state++;
				stateLoopCount = 0;
				//setDriveDirection(STOPPED,0.0);
				//inner_state = 0;
				//currentState = stage4;
			}
			break;
		case 9:
			if ((sensorData->ir4_1_state) == 1 && (sensorData->ir4_2_state == 0)) {
				if (stateLoopCount > 9) {
					setDriveDirection(STRAIGHT_BACKWARD,2.0);
					inner_state++;
				}
				stateLoopCount++;
			}
			else {
				stateLoopCount = 0;
			}
			break;
		case 10:
			if ((sensorData->ir4_1_state == 1) && (sensorData->ir4_2_state == 0)) {
				setDriveDirection(STOPPED,0.0);
				inner_state = 0;
				currentState = stage4;
				display_flag = 0;
			}
			break;*/

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
	currentState = start;
	inner_state = 0;
}
