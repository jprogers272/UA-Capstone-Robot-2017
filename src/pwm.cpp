#include "pwm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>

using namespace std;

PWM::PWM() {
	pwm_file_paths[0] = identifyFile(string(PWM0_FILE_PREFIX),PWM0A);
	pwm_file_paths[1] = identifyFile(string(PWM0_FILE_PREFIX),PWM0B);
	pwm_file_paths[2] = identifyFile(string(PWM1_FILE_PREFIX),PWM1A);
	pwm_file_paths[3] = identifyFile(string(PWM1_FILE_PREFIX),PWM1B);
	pwm_file_paths[4] = identifyFile(string(PWM2_FILE_PREFIX),PWM2A);
	pwm_file_paths[5] = identifyFile(string(PWM2_FILE_PREFIX),PWM2B);
}

void PWM::setDutyCycle(int duty_cycle, PWMchannel channel) {
	ofstream file_pwm;
	ostringstream file_path;
    file_path << pwm_file_paths[2*channel.device + channel.part] << "duty_cycle";
	file_pwm.open(file_path.str());
	file_pwm << duty_cycle;
	file_pwm.close();
}

void PWM::setPeriod(int period, PWMchannel channel) {
	ofstream file_pwm;
	ostringstream file_path;
    file_path << pwm_file_paths[2*channel.device + channel.part] << "period";
	file_pwm.open(file_path.str());
	file_pwm << period;
	file_pwm.close();
}

void PWM::setState(int state, PWMchannel channel) {
	ofstream file_pwm;
	ostringstream file_path;
    file_path << pwm_file_paths[2*channel.device + channel.part] << "state";
	file_pwm.open(file_path.str());
	file_pwm << state;
	file_pwm.close();
}

string PWM::identifyFile(string prefix, PWMchannel channel) {
	DIR *dir;
	struct dirent *dp;
	ostringstream return_path;
	
	dir = opendir(prefix.c_str());
	
	while ( (dp = readdir(dir)) != NULL) {
		string entry(dp->d_name);
		if ( !entry.compare(".") || !entry.compare("..") ) {
			//do nothing
		}
		else {
			//name should be pwmchip#
			return_path << prefix << entry << "/pwm" << channel.part << '/';
		}
	}
	closedir(dir);
	return return_path.str();
}