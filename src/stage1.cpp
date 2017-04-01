//Author(s): John Rogers

#include "stage1.hpp"
#include "analog.hpp"
#include "gpio.hpp"
#include "robotDefinitions.hpp"
#include "timing.hpp"

#include <iostream>

using namespace std;

Stage1::Stage1(void) {
	testGPIOs[0] = STAGE1_GPIO1;
	testGPIOs[1] = STAGE1_GPIO2;
	testGPIOs[2] = STAGE1_GPIO3;
	testGPIOs[3] = STAGE1_GPIO4;
	testGPIOs[4] = STAGE1_GPIO5;
	testGPIOs[5] = STAGE1_GPIO_COM;
	int i;
	for (i=0; i<6; i++) {
		setDirectionGPIO(testGPIOs[i],GPIO_INPUT);
	}
	currentComponent = 0;
}

void Stage1::energizeComponent(void) {
	setDirectionGPIO(testGPIOs[currentComponent],GPIO_OUTPUT);
	writeGPIO(testGPIOs[currentComponent],1);
}

void Stage1::deEnergizeComponent(void) {
	writeGPIO(testGPIOs[currentComponent],0);
	setDirectionGPIO(testGPIOs[currentComponent],GPIO_INPUT);
}

void Stage1::energizeCommon(void) {
	setDirectionGPIO(STAGE1_GPIO_COM,GPIO_OUTPUT);
	writeGPIO(STAGE1_GPIO_COM,1);
}

void Stage1::checkCapacitorDiode(int ignorePin) {
	int i;
	for (i=0; i<5; i++) {
		if (i == ignorePin) {
			continue;
		}
		if (components[i] == 0) {
			if (readGPIO(testGPIOs[i])) {
				components[i] = 5;
			}
			else {
				components[i] = 3;
			}
		}
		else if ((components[i] == 4) || (components[i] == 5)) {
			cout << "current component is " << i << " and GPIO through is " << readGPIO(testGPIOs[i]);
			if (!readGPIO(testGPIOs[i])) {
				components[i] = 5;
			}
			else {
				components[i] = 4;
			}
		}

	}
	writeGPIO(STAGE1_GPIO_COM,0);
	setDirectionGPIO(STAGE1_GPIO_COM,GPIO_INPUT);
}

void Stage1::identifyComponent(void) {
	int mV = readADC_mv(STAGE1_ADC);
	cout << "Vtest = " << mV << "mV\n";
	if (mV < 100) components[currentComponent] = 0;
	else if ( (mV > 150) && (mV < 600) ) components[currentComponent] = 2;
	else if ( (mV > 850) && (mV < 1400) ) components[currentComponent] = 5;
	else if ( (mV >= 1400) && (mV < 1600) ) components[currentComponent] = 4;
	else if (mV >= 1600) components[currentComponent] = 1;
	else components[currentComponent] = 0;

	writeGPIO(testGPIOs[currentComponent],0);
	setDirectionGPIO(testGPIOs[currentComponent],GPIO_INPUT);
}

int Stage1::detectProblems(void) {
	int i,j;
	int opens = 0;
	int shorts = 0;
	for (i=0; i<5; i++) {
		for (j=0; j<5; j++) {
			if ( (i != j) && (components[i] == components[j]) ) {
				if (components[i] == 3) {
					if (detectShort(testGPIOs[i],testGPIOs[j]))
						shorts++;
					else 
						opens++;
				}
				else
					shorts++;
			}
		}
	}
	if (shorts && opens) return 3;
	else if (opens && !shorts) return 2;
	else if (shorts && !opens) return 1;
	else return 0;
}

int Stage1::detectShort(int gpio1, int gpio2) {
	int shorted = 0;
	setDirectionGPIO(gpio1,GPIO_OUTPUT);
	writeGPIO(gpio1,1);
	if (readGPIO(gpio2)) {
		shorted = 1;
	}
	writeGPIO(gpio1,0);
	setDirectionGPIO(gpio1,GPIO_INPUT);
	return shorted;
}

void Stage1::zeroComponentArray(void) {
	components[0] = 0;
	components[1] = 0;
	components[2] = 0;
	components[3] = 0;
	components[4] = 0;
}
