#include "analog.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//return analog voltage, between 0 and 1800 mV
int readADC_mv(int channel) {
	int raw_value = readADC_raw(channel);
	return convertAnalog_raw(raw_value);
}

//return ADC value, between 0 and 4096
int readADC_raw(int channel) {
	ifstream file_analog;
	ostringstream file_path;
	file_path << ADC_FILE_PREFIX << channel << "_raw";
	file_analog.open(file_path.str());
	int adc_value;
	file_analog >> adc_value;
	return adc_value;
}

int convertAnalog_raw(int raw_value) {
	return (int) (((float)raw_value / 4095.0) * 1800);
}