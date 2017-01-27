//This code is specific to kernel version 4.4.9-ti-r25

#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define MAX_DUTY_CYCLE 50000
#define MAX_MAGNITUDE 0.25
//#define PWM1A_file "/sys/devices/platform/ocp/48302000.epwmss/48302200.pwm/pwm/pwmchip*/pwm0/"
//#define PWM1B_file "/sys/devices/platform/ocp/48302000.epwmss/48302200.pwm/pwm/pwmchip*/pwm1/"
//#define PWM2A_file "/sys/devices/platform/ocp/48304000.epwmss/48304200.pwm/pwm/pwmchip*/pwm0/"
//#define PWM2B_file "/sys/devices/platform/ocp/48304000.epwmss/48304200.pwm/pwm/pwmchip*/pwm1/"

#define PWM1_FILE_PREFIX "/sys/devices/platform/ocp/48302000.epwmss/48302200.pwm/pwm/"
#define PWM2_FILE_PREFIX "/sys/devices/platform/ocp/48304000.epwmss/48304200.pwm/pwm/"

#define PWM1A 0
#define PWM1B 1
#define PWM2A 2
#define PWM2B 3

#define DIR1A 89
#define DIR1B 11
#define DIR2A 75
#define DIR2B 73

char PWM1A_file[100];
char PWM1B_file[100];
char PWM2A_file[100];
char PWM2B_file[100];

void identifyFiles(void);
void writeMotor(float,int);
void setDutyCyclePWM(int,int);
void setPeriodPWM(int,int);
void setStatePWM(int,int);
void writeGPIO(int,int);
int readGPIO(int);
void setDirectionGPIO(int,int);

int main(int argc, char **argv) {
	if (argc != 5) {
		printf("Improper usage - run with four numbers 1 or 0.\n");
		return 1;
	}

	identifyFiles();
	
	float motor_magnitudes[4];
	
	setDirectionGPIO(DIR1A, 0);
	setDirectionGPIO(DIR1B, 0);
	setDirectionGPIO(DIR2A, 0);	
	setDirectionGPIO(DIR2B, 0);
	
	int i;
	for (i=1; i<5; i++) {
		if (argv[i][0] == '-') {
			motor_magnitudes[i-1] = -1.0 * MAX_MAGNITUDE;
		}
		else if (argv[i][0] == '0') {
			motor_magnitudes[i-1] = 0.0;
		}
		else if (argv[i][0] == '1') {
			motor_magnitudes[i-1] = 1.0 * MAX_MAGNITUDE;
		}
		else {
			printf("Improper usage - run with four numbers -1, 0, or 1.\n");
			return 1;
		}
	}

	//printf("Setting pwm periods to 50,000 ns.\n");
	
	setPeriodPWM(50000,PWM1A);
	setPeriodPWM(50000,PWM1B);
	setPeriodPWM(50000,PWM2A);
	setPeriodPWM(50000,PWM2B);

	//printf("writing pwms and direction pins\n");
	
	writeMotor(motor_magnitudes[0],PWM1A);
	writeMotor(motor_magnitudes[1],PWM1B);
	writeMotor(motor_magnitudes[2],PWM2A);
	writeMotor(motor_magnitudes[3],PWM2B);

	setStatePWM(1,PWM1A);
	setStatePWM(1,PWM1B);
	setStatePWM(1,PWM2A);
	setStatePWM(1,PWM2B);

	return 0;
}

void identifyFiles(void) {
	DIR *dir;
	struct dirent *dp;
	char file_end[20];
	dir = opendir(PWM1_FILE_PREFIX);
	while ( (dp = readdir(dir)) != NULL) {
		//printf("%s\n", dp->d_name);
		if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") ) {
			//do nothing
		}
		else {
			//name should be pwmchip#
			sprintf(file_end, "%s/pwm0/", dp->d_name);
			sprintf(PWM1A_file, "%s", PWM1_FILE_PREFIX);
			sprintf(PWM1B_file, "%s", PWM1_FILE_PREFIX);
			strcat(PWM1A_file, file_end);
			sprintf(file_end, "%s/pwm1/", dp->d_name);
			strcat(PWM1B_file, file_end);
		}
	}
	closedir(dir);
	//printf("%s\n%s\n",PWM1A_file, PWM1B_file);
	dir = opendir(PWM2_FILE_PREFIX);
	while ( (dp = readdir(dir)) != NULL) {
		//printf("%s\n", dp->d_name);
		if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") ) {
			//do nothing
		}
		else {
			//name should be pwmchip#
			sprintf(file_end, "%s/pwm0/", dp->d_name);
			sprintf(PWM2A_file, "%s", PWM2_FILE_PREFIX);
			sprintf(PWM2B_file, "%s", PWM2_FILE_PREFIX);
			strcat(PWM2A_file, file_end);
			sprintf(file_end, "%s/pwm1/", dp->d_name);
			strcat(PWM2B_file, file_end);
		}
	}
	closedir(dir);
	//printf("%s\n%s\n",PWM2A_file, PWM2B_file);
}

void writeMotor(float magnitude, int channel) {
	int pwm_channel = 0;
	int motor_direction = 0;
	int direction_pin = 0;
	int duty = 0;
	switch (channel) {
		case PWM1A:
			direction_pin = DIR1A;
			break;
		case PWM1B:
			direction_pin = DIR1B;
			break;
		case PWM2A:
			direction_pin = DIR2A;
			break;
		case PWM2B:
			direction_pin = DIR2B;
			break;
		default:
			return;
			break;
	}
	if (magnitude > 0) {
		motor_direction = 0;
	}
	else {
		motor_direction = 1;
		magnitude *= -1.0;
	}
	duty = (int) (magnitude * (float)MAX_DUTY_CYCLE);
	setDutyCyclePWM(duty, channel);
	writeGPIO(direction_pin, motor_direction);
}

void setDutyCyclePWM(int duty_cycle, int channel) {
	FILE *f_pwm;
	char file_path[100];
	char file_end[20];

	//printf("declared file strings\n");	

	if (channel == PWM1A) {sprintf(file_path,"%s",PWM1A_file);}
	else if (channel == PWM1B) {sprintf(file_path,"%s",PWM1B_file);}
	else if (channel == PWM2A) {sprintf(file_path,"%s",PWM2A_file);}
	else if (channel == PWM2B) {sprintf(file_path,"%s",PWM2B_file);}
	else {return;}


	//printf("set file path prefix correctly (I hope)\n");
	
	sprintf(file_end,"duty_cycle",channel);
	strcat(file_path,file_end);
	f_pwm = fopen(file_path,"w");
	fprintf(f_pwm,"%d",duty_cycle);
	fclose(f_pwm);
}

void setPeriodPWM(int period, int channel) {
	FILE *f_pwm;
	char file_path[100];
	char file_end[15];
	int prevPeriod;
	
	//printf("declared file strings\n");	

	if (channel == PWM1A) {sprintf(file_path,"%s",PWM1A_file);}
	else if (channel == PWM1B) {sprintf(file_path,"%s",PWM1B_file);}
	else if (channel == PWM2A) {sprintf(file_path,"%s",PWM2A_file);}
	else if (channel == PWM2B) {sprintf(file_path,"%s",PWM2B_file);}
	else {return;}
	
	//printf("set proper file path prefix\n");
	sprintf(file_end,"period",channel);
	strcat(file_path,file_end);
	
	//printf("opening period file\n");
	f_pwm = fopen(file_path,"r");
	//printf("%s\n",file_path);
	fscanf(f_pwm,"%d",&prevPeriod);
	if (period == prevPeriod) {
		fclose(f_pwm);
		return;
	}
	
	f_pwm = fopen(file_path,"w");
	fprintf(f_pwm,"%d",period);
	fclose(f_pwm);
}

void setStatePWM(int state, int channel) {
	FILE *f_pwm;
	char file_path[100];
	char file_end[15];
	int prevState;
	
	if (channel == PWM1A) {sprintf(file_path,"%s",PWM1A_file);}
	else if (channel == PWM1B) {sprintf(file_path,"%s",PWM1B_file);}
	else if (channel == PWM2A) {sprintf(file_path,"%s",PWM2A_file);}
	else if (channel == PWM2B) {sprintf(file_path,"%s",PWM2B_file);}
	else {return;}
	
	sprintf(file_end,"enable",channel);
	strcat(file_path,file_end);
	
	f_pwm = fopen(file_path,"w");
	fprintf(f_pwm,"%d",state);
	fclose(f_pwm);
}

void writeGPIO(int pin_number, int value) {
	FILE *f_gpio;
	char file_path[50] = "/sys/class/gpio/gpio";
	char file_end[9];
	sprintf(file_end,"%d/value",pin_number);
	strcat(file_path,file_end);
	f_gpio = fopen(file_path,"w");
	fprintf(f_gpio,"%d",value);
	fclose(f_gpio);
}

int readGPIO(int pin_number) {
	int gpio_value;
	FILE *f_gpio;
	char file_path[50] = "/sys/class/gpio/gpio";
	char file_end[9];
	sprintf(file_end,"%d/value",pin_number);
	strcat(file_path,file_end);
	f_gpio = fopen(file_path,"r");
	fscanf(f_gpio,"%d",&gpio_value);
	fclose(f_gpio);
	return gpio_value;
}

void setDirectionGPIO(int pin_number, int direction) {
	FILE *f_gpio;
	char file_path[50] = "/sys/class/gpio/gpio";
	char file_end[15];
	sprintf(file_end,"%d/direction",pin_number);
	strcat(file_path,file_end);
	f_gpio = fopen(file_path,"w");
	if (direction) fprintf(f_gpio,"in");
	else fprintf(f_gpio,"out");
	fclose(f_gpio);
}
