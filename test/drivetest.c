#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#define PI 3.141592

#define VBAT_MULTIPLIER 11.2

#define PWM_PERIOD 50000
#define MAX_DUTY_CYCLE 50000
#define MAX_MAGNITUDE 0.5

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
void processMecanum(float,float,float,float,float*);
void writeMotor(float,int,float);
void setDutyCyclePWM(int,int);
void setPeriodPWM(int,int);
void setStatePWM(int,int);
void writeGPIO(int,int);
int readGPIO(int);
void setDirectionGPIO(int,int);
int readADC(int);
void directionverify();
void driveme(char);

int motorDrive(float voltage_max, float velocity_translation, float angle_translation, float velocity_rotation) {
	// if (argc != 5) {
	// 	printf("Improper usage - run with four floats, max voltage, translation velocity, translation angle, and rotation velocity.\n");
	// 	printf("Max voltage - a float between 0.0 and 6.0.\n");
	// 	printf("Translation velocity - a float between -1.0 and 1.0.\n");
	// 	printf("Translation angle - a float between 0.0 and 360.0 (degrees).\n");
	// 	printf("Rotation velocity - a float between -1.0 and 1.0.\n");
	// 	return 1;
	// }
	
	
	if (voltage_max > 6.0) 
		voltage_max = 6.0;
	else if (voltage_max < 0.0) 
		voltage_max = 0.0;
	// float velocity_translation = atof(argv[2]);
	if (velocity_translation > 1.0) 
		velocity_translation = 1.0;
	else if (velocity_translation < -1.0) 
		velocity_translation = -1.0;
	// float angle_translation = atof(argv[3]);
	angle_translation *= PI/180.0;
	// float velocity_rotation = atof(argv[4]);
	if (velocity_rotation > 1.0) 
		velocity_rotation = 1.0;
	else if (velocity_rotation < -1.0) 
		velocity_rotation = -1.0;

	identifyFiles();
	
	setDirectionGPIO(DIR1A, 0);
	setDirectionGPIO(DIR1B, 0);
	setDirectionGPIO(DIR2A, 0);	
	setDirectionGPIO(DIR2B, 0);
		
//	printf("set directions\n");

	float *motor_voltages;
	motor_voltages = malloc(4*sizeof(float));
	processMecanum(voltage_max,velocity_translation,angle_translation,velocity_rotation,motor_voltages);

//	printf("mecanum wheel voltages calculated\n");
	
	float vbat = VBAT_MULTIPLIER * readADC(2) / 1000.0;


	printf("read battery voltage. %f volts\n",vbat);
	//printf("Setting pwm periods to 50,000 ns.\n");
	
	setPeriodPWM(PWM_PERIOD,PWM1A);
	setPeriodPWM(PWM_PERIOD,PWM1B);
	setPeriodPWM(PWM_PERIOD,PWM2A);
	setPeriodPWM(PWM_PERIOD,PWM2B);

//	printf("periods set\n");
	//printf("writing pwms and direction pins\n");
	
	writeMotor(motor_voltages[0],PWM1A,vbat);
	writeMotor(motor_voltages[1],PWM1B,vbat);
	writeMotor(motor_voltages[2],PWM2A,vbat);
	writeMotor(motor_voltages[3],PWM2B,vbat);

	setStatePWM(1,PWM1A);
	setStatePWM(1,PWM1B);
	setStatePWM(1,PWM2A);
	setStatePWM(1,PWM2B);

//	printf("PWMs enabled\n");

	directionverify();
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

void processMecanum(float maximum_voltage, float vel_trans, float angle_trans, float vel_rot, float *voltages_return) {
	voltages_return[0] = maximum_voltage * (vel_trans * sin(angle_trans + PI/4.0) + vel_rot);
	voltages_return[1] = maximum_voltage * (vel_trans * cos(angle_trans + PI/4.0) - vel_rot);
	voltages_return[2] = maximum_voltage * (vel_trans * cos(angle_trans + PI/4.0) + vel_rot);
	voltages_return[3] = maximum_voltage * (vel_trans * sin(angle_trans + PI/4.0) - vel_rot);
	
	//check if any voltage magnitudes are greater than the provided maximum, and scale accordingly
	int i;
	float calculated_max = fabs(voltages_return[0]);
	for (i=1; i<4; i++) {
		if (fabs(voltages_return[i]) > calculated_max)
			calculated_max = fabs(voltages_return[i]);
	}
	if (calculated_max > maximum_voltage) {
		for (i=0; i<4; i++) {
			voltages_return[i] = voltages_return[i] * maximum_voltage / calculated_max;
		}
	}
}

void writeMotor(float voltage, int channel, float vbat) {
//	printf("begin writeMotor function\n");
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
	if (voltage > 0) {
		motor_direction = 0;
	}
	else {
		motor_direction = 1;
		voltage *= -1.0;
	}
	duty = (int) (voltage / vbat * PWM_PERIOD);
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

int readADC(int channel) {
	FILE *an_file;
	char file_path[100] = "/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc/iio:device0/in_voltage";
	char channel_str[10];
	sprintf(channel_str,"%d_raw",channel);
	strcat(file_path,channel_str);

	int adc_value;
	an_file = fopen(file_path, "r");
	//fseek(an_value,0,SEEK_SET);
	fscanf(an_file,"%d",&adc_value);
	fclose(an_file);
	
	//printf("value: %d,",adc_value);
	adc_value = (int) (((float)adc_value / 4095.0) * 1800);
	//returns actual voltage in mV (lose resolution, change to out of 4096)
	return adc_value;
}

void directionverify(){ //used at the end of the program when prompting user for input
	char direction;
	int check = 0;
	printf("Drive Direction? (w = forward, s = reverse, a = left, d = right)\n ");
	printf("y = NW, u = NE, h = SW, j = SE: ");
	scanf("\n%c", &direction);
	//fprintf(stdin, "%c", '\n');
	//fflush(stdin);
	if (direction != 'w'){ //foward
		check++;
	}
	if (direction != 's'){ //reverse
		check++;
	}
	if (direction != 'a'){ //left
		check++;
	}
	if (direction != 'd'){ //right
		check++;
	}
	if (direction != 'y'){ //NW
		check++;
	}
	if (direction != 'u'){ //NE
		check++;
	}
	if (direction != 'h'){ //SW
		check++;
	}
	if (direction != 'j'){ //SE
		check++;
	}

	if (direction == 'c'){ //exit case
		check++;
	}

	if (check < 9){
		struct timespec time_des;
		time_des.tv_sec = 0;
		time_des.tv_nsec = 10000000;
		nanosleep(time_des, NULL);
		driveme(direction);
	}

	else{
		directionverify(); //the entire reason for this function's existence is for this
		                 //recursive call and the safety it brings.
	}
	
	driveme(direction);
}

void driveme(char dir){
	//if dir == w
	//drive forward
	if(dir == 'w'){
		float voltage_max = 3.0;
		float velocity_translation = 1.0;
		float angle_translation = 0.0;
		float velocity_rotation = 0.0;

		motorDrive(voltage_max, velocity_translation, angle_translation, velocity_rotation);
	}

	//if dir == s
	//drive reverse
	if(dir == 's'){
		float voltage_max = 3.0;
		float velocity_translation = -1.0;
		float angle_translation = 0.0;
		float velocity_rotation = 0.0;

		motorDrive(voltage_max, velocity_translation, angle_translation, velocity_rotation);

	}

	//if dir == a
	//drive left
	if(dir == 'a'){
		float voltage_max = 3.0;
		float velocity_translation = 3010.0;
		float angle_translation = -90.0;
		float velocity_rotation = 0.0;

		motorDrive(voltage_max, velocity_translation, angle_translation, velocity_rotation);

	}

	//if dir == d
	//drive right
	if(dir == 'd'){
		float voltage_max = 3.0;
		float velocity_translation = 3010.0;
		float angle_translation = 90.0;
		float velocity_rotation = 0.0;

		motorDrive(voltage_max, velocity_translation, angle_translation, velocity_rotation);

	}

	//if dir = y
	//drive NW
	if(dir == 'y'){
		float voltage_max = 3.0;
		float velocity_translation = 3010.0;
		float angle_translation = -45.0;
		float velocity_rotation = 0.0;

		motorDrive(voltage_max, velocity_translation, angle_translation, velocity_rotation);

	}

	//if dir = u
	//drive NE
	if(dir == 'u'){
		float voltage_max = 3.0;
		float velocity_translation = 3010.0;
		float angle_translation = 45.0;
		float velocity_rotation = 0.0;

		motorDrive(voltage_max, velocity_translation, angle_translation, velocity_rotation);

	}

	//if dir == h
	//drive SW
	if(dir == 'h'){
		float voltage_max = 3.0;
		float velocity_translation = 3010.0;
		float angle_translation = -135.0;
		float velocity_rotation = 0.0;

		motorDrive(voltage_max, velocity_translation, angle_translation, velocity_rotation);

	}

	//if dir == j
	//drive SE
	if(dir == 'j'){
		float voltage_max = 3.0;
		float velocity_translation = 3010.0;
		float angle_translation = 135.0;
		float velocity_rotation = 0.0;

		motorDrive(voltage_max, velocity_translation, angle_translation, velocity_rotation);

	}

	//kill powah
	if(dir == 'c'){
		float voltage_max = 3.0;
		float velocity_translation = 0.0;
		float angle_translation = 0.0;
		float velocity_rotation = 0.0;

		motorDrive(voltage_max, velocity_translation, angle_translation, velocity_rotation);

	}

	directionverify();
}

int main(void){
	directionverify();
}

