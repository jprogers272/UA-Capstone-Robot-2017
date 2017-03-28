#include <stdio.h>
#include <string.h>
#include <time.h>

//112, 7, 15, 113, 14, 51
/*#define TEST1 15
#define TEST2 115
#define TEST3 2
#define TEST4 49
#define TEST5 3
#define TEST6 14
#define BUTTON 112*/
#define TEST1 20
#define TEST2 115
#define TEST3 14
#define TEST4 117
#define TEST5 49
#define TEST6 7

int testComponent(int,struct timespec*); 
int componentID(int);
int detectProblems(int*);
int detectShort(int,int);
int detectShortsAll();
int readADC(int);
void writeGPIO(int,int);
int readGPIO(int);
void setDirGPIO(int,int);

int main (void) {
	//setDirGPIO(7,0);
	//writeGPIO(7,1);
	int adc_value, i, counter;
	int test_pins[5] = {TEST1,TEST2,TEST3,TEST4,TEST5};
	printf("pins: %d, %d, %d, %d, %d\n",test_pins[0],test_pins[1],
		test_pins[2],test_pins[3],test_pins[4]);
	counter = 0;
	struct timespec time_des;
	struct timespec time_des_test;
	time_des.tv_sec = 0;
	time_des.tv_nsec = 10000000; //10ms
	time_des_test.tv_sec = 0;
	time_des_test.tv_nsec = 10000000;
	while (1) {
		//if (readGPIO(BUTTON)) {
			/*setDirGPIO(2,0);
			setDirGPIO(15,1);
			writeGPIO(2,1);
			nanosleep(&time_des,NULL);
			int component = componentID(readADC(0));
			if (component == 0) {
				setDirGPIO(2,1);
				setDirGPIO(15,0);
				writeGPIO(15,1);
				nanosleep(&time_des_test,NULL);
				if (readGPIO(2)) {component = 5;}
				else {component = 3;}
			}*/
			int component[5];
			component[0] = testComponent(TEST1,&time_des); 
			component[1] = testComponent(TEST2,&time_des); 
			component[2] = testComponent(TEST3,&time_des); 
			component[3] = testComponent(TEST4,&time_des); 
			component[4] = testComponent(TEST5,&time_des); 
			int problem = detectProblems(component);
			if (problem == 3)
				printf("Probe is not fully engaged and "
					"some test leads are shorted.\n");
			else if (problem == 2)
				printf("Probe is not fully engaged.\n");
			else if (problem == 1)
				printf("Some test leads are shorted.\n");
			else if (problem == 0) {
				printf("Test is acceptable.\n");
				int j;
				for (j=0; j<5; j++) {
					printf("%d: ",j+1);
					switch (component[j]) {
						case 0:
							break;
						case 1:
							printf("Wire\n");
							break;
						case 2:
							printf("Resistor\n");
							break;
						case 3:
							printf("Capacitor\n");
							break;
						case 4:
							printf("Inductor\n");
							break;
						case 5:
							printf("Diode\n");
							break;
					}	
				}
				printf("Code: %d %d %d %d %d\n",component[0], 
					component[1],component[2],
					component[3],component[4]);
			}
			//writeGPIO(2,0);
		//}
		//adc_value = readADC(0);
		//printf("\rAIN0: %d mV      ",adc_value);
		nanosleep(&time_des,NULL);
		//writeGPIO(7,readGPIO(14));
		//if (readGPIO(20)) break;
	}
	printf("\n");
	//writeGPIO(7,0);
	return 0;
}

int testComponent(int gpio, struct timespec *time_des) {
	setDirGPIO(TEST1,1);
	setDirGPIO(TEST2,1);
	setDirGPIO(TEST3,1);
	setDirGPIO(TEST4,1);
	setDirGPIO(TEST5,1);
	setDirGPIO(TEST6,1);

	setDirGPIO(gpio,0);
	writeGPIO(gpio,1);

	nanosleep(time_des,NULL);
	int component = componentID(readADC(0));
	writeGPIO(gpio,0);
	if (component == 0) {
		setDirGPIO(gpio,1);
		setDirGPIO(TEST6,0);
		writeGPIO(TEST6,1);
		nanosleep(time_des,NULL);
		if (readGPIO(gpio)) {component = 5;}
		else {component = 3;}
		writeGPIO(TEST6,0);
	}
	setDirGPIO(TEST6,1);
	return component;
}

int componentID(int mV) {
	//printf("Vtest = %dmV\n",mV);
	if (mV < 100) return 0;
	else if ( (mV > 150) && (mV < 600) ) return 2;
	else if ( (mV > 850) && (mV < 1450) ) return 4;
	else if ( (mV > 1450) && (mV < 1600) ) return 5;
	else if (mV > 1600) return 1;
	else return 0;
}

int detectProblems(int *components) {
	int i,j;
	int opens = 0;
	int shorts = 0;
	int test_gpios[5] = {TEST1,TEST2,TEST3,TEST4,TEST5};
	for (i=0; i<5; i++) {
		for (j=0; j<5; j++) {
			if ( (i != j) && (components[i] == components[j]) ) {
				if (components[i] == 3) {
					if (detectShort(test_gpios[i],test_gpios[j]))
						shorts++;
					else 
						opens++;
				}
				else
					shorts++;
			}
		}
	}
	if (shorts && opens) return 3;
	else if (opens && !shorts) return 2;
	else if (shorts && !opens) return 1;
	else return 0;
}

int detectShort(int gpio1, int gpio2) {
	int shorted = 0;
	setDirGPIO(gpio1,0);
	writeGPIO(gpio1,1);
	if (readGPIO(gpio2)) {
		shorted = 1;
		//printf("GPIOs %d and %d are shorted together!\n",gpio1,gpio2);
	}
	writeGPIO(gpio1,0);
	setDirGPIO(gpio1,1);
	return shorted;
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
	//printf("%d\n",adc_value);
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
