#include "gpio.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void writeGPIO(int gpio_number, int value) {
	ofstream file_gpio;
	ostringstream file_path;
    file_path << GPIO_FILE_PREFIX << gpio_number << "/value";
	file_gpio.open(file_path.str());
	file_gpio << value;
	file_gpio.close();
}

int readGPIO(int gpio_number) {
	ifstream file_gpio;
	ostringstream file_path;
	file_path << GPIO_FILE_PREFIX << gpio_namber << "/value";
	file_gpio.open(file_path.str());
	int value;
	file_gpio >> value;
	file_gpio.close();
}

void setDirectionGPIO) {
	ofstream file_gpio;
	ostringstream file_path;
    file_path << GPIO_FILE_PREFIX << gpio_number << "/direction";
	file_gpio.open(file_path.str());
	if (direction){
		file_gpio << "in";
	}
	else {
		file_gpio << "out"
	}
	file_gpio.close();
}