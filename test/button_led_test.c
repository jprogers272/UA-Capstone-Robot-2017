#include <stdio.h>
#include <time.h>

int main (void) {
	FILE *file = fopen("/sys/class/gpio/gpio7/direction", "w");
	fprintf(file, "out");
	fclose(file);
	FILE *f_LED = fopen("/sys/class/gpio/gpio7/value","r");
	int LED_value;
	fscanf(f_LED,"%d",&LED_value);
	fclose(f_LED);

	struct timespec time_des, time_rem;
	time_des.tv_sec = 0;
	time_des.tv_nsec = 100000000;
	time_rem.tv_nsec = 0; //not sure what this is doing	

	int button = 0;
	int i = 0;
	FILE *f_BUTTON;
	while (button == 0) {
		f_LED = fopen("/sys/class/gpio/gpio7/value","w");
		LED_value = !LED_value;
		fprintf(f_LED,"%d",LED_value);
		fclose(f_LED);
		//getchar();
		//for(i=0; i<100000000; i++); //should be about 200ms
		nanosleep(&time_des, &time_rem);
	//	printf("Time: elapsed: %ld,  rem: %ld ns\n",
	//		time_des.tv_nsec - time_rem.tv_nsec, time_rem.tv_nsec);
		f_BUTTON = fopen("/sys/class/gpio/gpio20/value","r");
		fscanf(f_BUTTON,"%d",&button);
		fclose(f_BUTTON);
	}
	return 0;
}
