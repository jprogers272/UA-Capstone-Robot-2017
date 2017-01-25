#include <stdio.h>
#include <string.h>
#include <time.h>

int IR_gpio[12] = {8, 76, 78, 74, 86, 87, 10, 9, 66, 69, 47, 27};

void writeGPIO(int,int);
int readGPIO(int);
void setDirGPIO(int,int);

int main (void) {
	int i;
	struct timespec time_des;
	time_des.tv_sec = 0;
	time_des.tv_nsec = 10000000; //10ms
	
	while (1) {
		for (i=0; i<12; i++) {
			printf("%d, ",readGPIO(IR_gpio[i]));
		}
		printf("\n");
		nanosleep(&time_des,NULL);
	}
	printf("\n");
	return 0;
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

void setDirGPIO(int pin_number, int direction) {
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
