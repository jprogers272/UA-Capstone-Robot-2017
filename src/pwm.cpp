#include "pwm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>

using namespace std;

//cant use constructor initialization because the set functions will then think the values are already set correctly
PWM::PWM(PWMchannel channel, int duty_cycle, int period, int state) {
	this->duty_cycle = -1;
	this->period = -1;
	this->state = -1;
	PWM::pwm_file_path = identifyFile(string(PWM0_FILE_PREFIX),channel);
	PWM::setDutyCycle(duty_cycle);
	PWM::setPeriod(period);
	PWM::setState(state);
	this->duty_cycle = duty_cycle;
	this->period = period;
	this->state = state;
	
}

void PWM::setDutyCycle(int duty_cycle) {
	if (duty_cycle == this->duty_cycle) {
		return;
	}
	this->duty_cycle = duty_cycle;
	ofstream file_pwm;
	ostringstream file_path;
    file_path << pwm_file_path << "duty_cycle";
	file_pwm.open(file_path.str());
	file_pwm << duty_cycle;
	file_pwm.close();
}

void PWM::setPeriod(int period) {
	if (period == this->period) {
		return;
	}
	this->period = period;
	ofstream file_pwm;
	ostringstream file_path;
    file_path << pwm_file_path << "period";
	file_pwm.open(file_path.str());
	file_pwm << period;
	file_pwm.close();
}

void PWM::setState(int state) {
	if (state == this->state) {
		return;
	}
	this->state = state;
	ofstream file_pwm;
	ostringstream file_path;
    file_path << pwm_file_path << "state";
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