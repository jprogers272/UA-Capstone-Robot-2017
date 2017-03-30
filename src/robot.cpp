//Author(s): John Rogers

#include "gpio.hpp"
#include "robot.hpp"
#include "angleControl.hpp"
#include "i2cbus.hpp"
#include "robotDefinitions.hpp"
#include "sensors.hpp"
#include "timing.hpp"
#include "display.hpp"

#include <iostream>

using namespace std;

Robot::Robot() :
	wheel_1(WHEEL1_PWM,WHEEL1_DIR),
	wheel_2(WHEEL2_PWM,WHEEL2_DIR),
	wheel_3(WHEEL3_PWM,WHEEL3_DIR),
	wheel_4(WHEEL4_PWM,WHEEL4_DIR),
	slapper(SLAPPER_PWM,SLAPPER_DIR),
	angle_controller(PGAIN,IGAIN,DGAIN),
	i2c_bus(2),
	disp(&i2c_bus),
	sensors(&i2c_bus)
{
	sensorData = new SensorData;
	drive_voltages = new float[4];

	resetRobot();
	start_switch_flag = 0;

	disp.clearDisplayAll();
}

void Robot::resetRobot(void) {
	currentState = start;
	nextState = stage1_solving;
	inner_state = 0;
	stateLoopCount = 0;

	display_flag = 0;
	end_thread_flag = 1;

	gyroAverageZ = 0.0;
	gyroAverageY = 0.0;
	gyroAverageX = 0.0;
	zeroVoltages();

	angle_controller.setSetpoint(0.0);
	angle_controller.setAngle(0.0);

	stage1.zeroComponentArray();
	writeGPIO(ENABLEDRIVER,0);
}

int Robot::robotLogic(void) {
//	cout << "current state: " << currentState << endl;
//	cout << "inner state: " << inner_state << endl;
	sensors.getAllSensors(sensorData);
	if (readGPIO(START_SWITCH_GPIO) == 1) {
		resetRobot();
	}
	switch (currentState) {
		case start:
			//disp.writeCenter("Robot Starting Up...",0);
			//disp.writeDisplay();
			start_logic();
			//disp.clearDisplay();
			break;
		case zero_gyro:
			//disp.writeCenter("Zeroing Gyro...",0);
			//disp.writeDisplay();
			zero_gyro_logic();
			//disp.clearDisplay();
			break;
		case pre_stage1:
			//disp.writeCenter("Prepping Stage 1...",0);
			//disp.writeDisplay();
			pre_stage1_logic();
			break;
		case stage1_solving:
			//disp.writeCenter("Solving Stage 1...",1);
			//disp.writeDisplay();
			stage1_logic();
			break;
		case post_stage1:
			//disp.writeCenter("Leaving Stage 1...",2);
			//disp.writeDisplay();
			post_stage1_logic();
			//disp.clearDisplay();
			break;
		case pre_stage2:
			//disp.writeCenter("Prepping Stage 2...",0);
			//disp.writeDisplay();
			pre_stage2_logic();
			break;
		case average_compass:
			//disp.writeCenter("Averaging Compass...",1);
			//disp.writeDisplay();
			average_compass_logic();
			break;
		case stage2:
			//disp.writeCenter("Completing Stage 2...",2);
			//disp.writeDisplay();
			stage2_logic();
			break;
		case post_stage2:
			//disp.writeCenter("Leaving Stage 2...",3);
			//disp.writeDisplay();
			post_stage2_logic();
			//disp.clearDisplay();
			break;
		case pre_stage3:
			//disp.writeCenter("Prepping Stage 3...",0);
			//disp.writeDisplay();
			pre_stage3_logic();
			break;
		case stage3_solving:
			//disp.writeCenter("Solving Stage 3...",1);
			//disp.writeDisplay();
			stage3_logic();
			break;
		case post_stage3:
			//disp.writeCenter("Leaving Stage 3...",2);
			//disp.writeDisplay();
			post_stage3_logic();
			//disp.clearDisplay();
			break;
		case pre_stage4:
			//disp.writeCenter("Prepping Stage 4...",0);
			//disp.writeDisplay();
			pre_stage4_logic();
			break;
		case stage4:
			//disp.writeCenter("Completing Stage 4...",1);
			//disp.writeDisplay();
			stage4_logic();
			//disp.clearDisplay();
			break;
		case finish:
			//disp.writeCenter("Shutting Down...",0);
			finish_logic();
			//return 1;
			break;
	}
	robotWait(0,LOOP_TIME_MS);
	return 0;
}

void Robot::setOutputs(void) {
	//cout << "battery voltage is " << sensorData->battery_voltage << endl;
	wheel_1.setVoltage(drive_voltages[0],sensorData->battery_voltage);
	wheel_2.setVoltage(drive_voltages[1],sensorData->battery_voltage);
	wheel_3.setVoltage(drive_voltages[2],sensorData->battery_voltage);
	wheel_4.setVoltage(drive_voltages[3],sensorData->battery_voltage);
	slapper.setVoltage(slapper_voltage,sensorData->battery_voltage);
}

void Robot::zeroVoltages(void) {
	drive_voltages[0] = 0.0;
	drive_voltages[1] = 0.0;
	drive_voltages[2] = 0.0;
	drive_voltages[3] = 0.0;
	slapper_voltage = 0.0;
}
