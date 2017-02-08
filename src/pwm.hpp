#ifndef PWM_HPP
#define PWM_HPP

#include <string> //for identifyFile function prototype

#define PWM0_FILE_PREFIX "/sys/devices/platform/ocp/48300000.epwmss/48302200.pwm/pwm/"
#define PWM1_FILE_PREFIX "/sys/devices/platform/ocp/48302000.epwmss/48302200.pwm/pwm/"
#define PWM2_FILE_PREFIX "/sys/devices/platform/ocp/48304000.epwmss/48304200.pwm/pwm/"

struct PWMchannel { int device; int part; };

PWMchannel const PWM0A = {0,0};
PWMchannel const PWM0B = {0,1};
PWMchannel const PWM1A = {1,0};
PWMchannel const PWM1B = {1,1};
PWMchannel const PWM2A = {2,0};
PWMchannel const PWM2B = {2,1};

class PWM {
	private:
		//file path for each individual pwm channel
		std::string pwm_file_paths[6];
		
	public:
		PWM();
		void setDutyCycle(int,PWMchannel);
		void setPeriod(int,PWMchannel);
		void setState(int,PWMchannel);
		
	private:
		static std::string identifyFile(std::string,PWMchannel);
};

#endif