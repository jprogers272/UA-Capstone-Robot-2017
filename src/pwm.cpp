#include "pwm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>

using namespace std;

//cant use constructor initialization because the set functions will then think the values are already set correctly
PWM::PWM(PWMchannel channel, int period, int duty_cycle, int state) {
	this->duty_cycle = -1;
	this->period = -1;
	this->state = -1;
	if (channel.device == 0) {file_prefix = string(PWM0_FILE_PREFIX);}
	else if (channel.device == 1) {file_prefix = string(PWM1_FILE_PREFIX);}
	else /*channel.device == 2*/ {file_prefix = string(PWM2_FILE_PREFIX);}
	pwm_file_path = identifyFile(file_prefix,channel);
	setPeriod(period);
	setDutyCycle(duty_cycle);
	setState(state);
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
//	cout << "writing " << duty_cycle << " to " << file_path.str() << endl;
	file_pwm.open(file_path.str().c_str());
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
	file_pwm.open(file_path.str().c_str());
	file_pwm << period;
	file_pwm.close();
}

void PWM::setState(int state) {
	if (state == this->state) {
//		cout << "state unchanging" << endl;
		return;
	}
	this->state = state;
	ofstream file_pwm;
	ostringstream file_path;
	file_path << pwm_file_path << "enable";	
//	cout << "writing " << this->state << " to " << file_path.str() << endl;
	file_pwm.open(file_path.str().c_str());
	file_pwm << this->state;
	file_pwm.flush();
	file_pwm.close();
}

string PWM::identifyFile(string prefix, PWMchannel channel) {
//	cout << "Identifying files" << endl;
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
