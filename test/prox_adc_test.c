#include <stdio.h>
#include <string.h>
#include <time.h>

int readADC(int);
void writeGPIO(int,int);
int readGPIO(int);

int main (void) {
	writeGPIO(7,1);
	int adc_value, i, counter;
	counter = 0;
	struct timespec time_des;
	time_des.tv_sec = 0;
	time_des.tv_nsec = 1000000; //10ms
	while (1) {
		adc_value = readADC(0);
		printf("\rAIN0: %d mV      ",adc_value);
		nanosleep(&time_des,NULL);
		writeGPIO(7,readGPIO(14));
		if (readGPIO(20)) break;
	}
	printf("\n");
	writeGPIO(7,0);
	return 0;
}

int readADC(int channel) {
	FILE *an_value;
	char file_path[50] = "/sys/devices/ocp.3/helper.15/AIN";
	char channel_str[5];
	sprintf(channel_str,"%d",channel);
	strcat(file_path,channel_str);

	int adc_value;
	an_value = fopen(file_path, "r");
	fseek(an_value,0,SEEK_SET);
	fscanf(an_value,"%d",&adc_value);
	fclose(an_value);
	
	return adc_value;
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
