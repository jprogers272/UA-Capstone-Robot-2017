#include "Stage3OP.hpp"

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



void Stage3Op(int c1, int c2, int c3, int c4, int c5){

	int i; //loop variable

	//delay structuring
	struct timespec time_des, time_rem;
	time_des.tv_sec = 0;
	time_des.tv_nsec = 10000000; //10ms
	time_rem.tv_nsec = 0; //not sure what this is doing



	setDirectionGPIO(45, 1); //STEP = GPIO PIN 45
	setDirectionGPIO(44, 1); //DIR  = GPIO PIN 44

	//First code, c1, is to be turned c1 times clockwise
	writeGPIO(44, int); //DIR = ROTATE CLOCKWISE
	for (i = 0; i < 200*c1; i++){
		writeGPIO(45, 1); //STEP
		//delay
		nanosleep(&time_des, &time_rem);
		writeGPIO(45, 0);  //!STEP
	}

	//Second code, c2, is to be turned c2 times counter-clockwise
	writeGPIO(44, int); //DIR = ROTATE COUNTER-CLOCKWISE
	for (i = 0; i < 200*c2; i++){
		writeGPIO(gpio,int); //STEP
		//delay
		nanosleep(&time_des, &time_rem);
		writeGPIO(gpio,int)  //!STEP
	}

	//Third code, c3, is to be turned c3 times clockwise
	writeGPIO(44, int); //DIR = ROTATE CLOCKWISE
	for (i = 0; i < 200*c3; i++){
		writeGPIO(45, 1); //STEP
		//delay
		nanosleep(&time_des, &time_rem);
		writeGPIO(45, 0);  //!STEP
	}

	//Fourth code, c4, is to be turned c4 times counter-clockwise
	writeGPIO(44, int); //DIR = ROTATE COUNTER-CLOCKWISE
	for (i = 0; i < 200*c4; i++){
		writeGPIO(45, 1); //STEP
		//delay
		nanosleep(&time_des, &time_rem);
		writeGPIO(45, 0);  //!STEP
	}

	//Fifth code, c5, is to be turned c5 times clockwise
	writeGPIO(44, int); //DIR = ROTATE COUNTER-CLOCKWISE
	for (i = 0; i < 200*c5; i++){
		writeGPIO(45, 1); //STEP
		//delay
		nanosleep(&time_des, &time_rem);
		writeGPIO(45, 0);  //!STEP
	}

	return;

}