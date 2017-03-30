#include "Stage3Op.hpp"
#include "gpio.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "timing.hpp"

/***************************************************
 * Stage3OP.cpp - An IEEE SECON 2017 program file  *
 * Author(s): Brandon Fikes                        *
 * Intent: Drive the stepper motor for Stage 3 by  *
 * taking in 5 integer values corresponding to the *
 * codes obtained from Stage 1 of the competition  *
 **************************************************/

 /***********************************************
  * With the current stepper driver we utilize, *
  * A2 = BLUE                                   *
  * A1 = RED                                    *
  * B1 = GREEN                                  *
  * B2 = BLACK                                  *
  ***********************************************/


#define DELAY_MOTOR 1 //5 milliseconds
#define DELAY_NEXT  2 //2 seconds

#define STEP 45
#define DIRECTION 44
#define SLEEP 46

//robotWait is used as follows: robotWait(seconds, milliseconds)


void Stage3Op(int c1, int c2, int c3, int c4, int c5){

	int i; //loop variable

	//configure pins used
	setDirectionGPIO(STEP, 0); //STEP = GPIO PIN 45 (OUTPUT)
	setDirectionGPIO(DIRECTION, 0); //DIR  = GPIO PIN 44 (OUTPUT)
	setDirectionGPIO(SLEEP, 0); //ENABLE = GPIO PIN 26 (OUTPUT)

	//wake the stepper driver from sleep
	writeGPIO(SLEEP, 1);

	//First code, c1, is to be turned c1 times clockwise
	writeGPIO(DIRECTION, 0); //DIR = ROTATE CLOCKWISE
	writeGPIO(STEP, 1);
	for (i = 0; i < 200*c1; i++){
		writeGPIO(STEP, 0); //STEP
		//delay
		robotWait(0, DELAY_MOTOR);
		writeGPIO(STEP, 1);  //!STEP
		robotWait(0, DELAY_MOTOR);
	}

	

	//settle
	robotWait(DELAY_NEXT, 0);

	//Second code, c2, is to be turned c2 times counter-clockwise
	writeGPIO(DIRECTION, 1); //DIR = ROTATE COUNTER-CLOCKWISE
	for (i = 0; i < 200*c2; i++){
		writeGPIO(STEP, 1); //STEP
		//delay
		robotWait(0, DELAY_MOTOR);
		writeGPIO(STEP, 0);  //!STEP
		robotWait(0, DELAY_MOTOR);
	}

	/*	
	//settle
	robotWait(DELAY_NEXT, 0);

	//Third code, c3, is to be turned c3 times clockwise
	writeGPIO(DIRECTION, 0); //DIR = ROTATE CLOCKWISE
	for (i = 0; i < 200*c3; i++){
		writeGPIO(STEP, 1); //STEP
		//delay
		robotWait(0, DELAY_MOTOR);
		writeGPIO(STEP, 0);  //!STEP
		robotWait(0, DELAY_MOTOR);
	}
	
	//settle
	robotWait(DELAY_NEXT, 0);

	//Fourth code, c4, is to be turned c4 times counter-clockwise
	writeGPIO(DIRECTION, 1); //DIR = ROTATE COUNTER-CLOCKWISE
	for (i = 0; i < 200*c4; i++){
		writeGPIO(STEP, 1); //STEP
		//delay
		robotWait(0, DELAY_MOTOR);
		writeGPIO(STEP, 0);  //!STEP
		robotWait(0, DELAY_MOTOR);
	}

	//settle
	robotWait(DELAY_NEXT, 0);
	
	//Fifth code, c5, is to be turned c5 times clockwise
	writeGPIO(DIRECTION, 0); //DIR = ROTATE COUNTER-CLOCKWISE
	for (i = 0; i < 200*c5; i++){
		writeGPIO(STEP, 1); //STEP
		//delay
		robotWait(0, DELAY_MOTOR);
		writeGPIO(STEP, 0);  //!STEP
		robotWait(0, DELAY_MOTOR);
	}
	
	//settle
	robotWait(DELAY_NEXT, 0);

	
	//put the stepper driver to sleep
	*/
	writeGPIO(SLEEP, 0);
	return;
}

int main(){
	Stage3Op(1, 2, 3, 4, 5);
	return 0;
}
