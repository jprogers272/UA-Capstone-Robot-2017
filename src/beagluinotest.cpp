#include "gpio.hpp"
#include "robotDefinitions.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "timing.hpp"

using namespace std;

void beagluinoTest(int c1){
	int looper;
	int current_state;
	int previous_state = 0;
	//int waiting = 0;
	cout << "Driver written high" << endl;
	writeGPIO(STEPDIRECTION, 1);
	for (looper = 0; looper < c1; looper++){
		previous_state = 0;
		writeGPIO(STEP_RUN, 1);
		current_state = readGPIO(PROCESS_UPDATE);
		while(current_state != 1 && previous_state == 0){
			previous_state = current_state;
			current_state = readGPIO(PROCESS_UPDATE);
			cout << "waiting" << endl;
		}
		cout << "PROCESS UPDATE HIGH" << endl;
		writeGPIO(STEP_RUN, 0);
		robotWait(0, 10);
	}

}

int main(){
	setDirectionGPIO(STEP_RUN, 0);
	setDirectionGPIO(STEPDIRECTION, 0);
	setDirectionGPIO(PROCESS_UPDATE, 1);
	setDirectionGPIO(ENABLEDRIVER, 0);
	writeGPIO(ENABLEDRIVER, 1);
	beagluinoTest(3);
	writeGPIO(ENABLEDRIVER, 0);
	return 0;
}
