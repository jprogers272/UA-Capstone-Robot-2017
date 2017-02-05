#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <time.h>

#define ADDR 0x6a
#define BUFFER_SIZE 40

unsigned char* readRegisters(int,int,int);
int writeValue(unsigned char,int);
short combineRegisters(unsigned char,unsigned char);

//int length;
//unsigned char buffer[BUFFER_SIZE] = {0};

int main (void) {
	int file_i2c;
	if ((file_i2c = open("/dev/i2c-2", O_RDWR)) < 0) {
		printf("couldnt open bus file\n");
		return 1;
	}
	if (ioctl(file_i2c, I2C_SLAVE, ADDR) < 0) {
		printf("Failed to get the bus\n");
		return 1;
	}
	char writeBuffer[1] = {0x00};
	if (write(file_i2c, writeBuffer, 1) != 1) {
		printf("Failed to reset the read address\n");
		return 1;
	}
	char* readBuffer;
	/*if(read(file_i2c, readBuffer, BUFFER_SIZE) != BUFFER_SIZE) {
		printf("Failed to read in buffer\n");
		return 1;
	}*/
	readBuffer = readRegisters(0x0F,0x0F,file_i2c);
	printf("The device ID is: 0x%02x\n", readBuffer[0]);

	struct timespec time_des;
	time_des.tv_nsec = 10000000; //10 ms
	time_des.tv_sec = 0;
	
	float accel[4];
	float gyro[3];
	float temperature;

	int i;
	FILE *gyro_data = fopen("/home/gyro.csv","w");
	fprintf(gyro_data,"z axis dps\n");
	fclose(gyro_data);
	
	free(readBuffer);
	while(1) {
		gyro_data = fopen("/home/gyro.csv","a");
		readBuffer = readRegisters(0x20,0x2D,file_i2c);
		
		temperature = 25.0 + combineRegisters(readBuffer[1],readBuffer[0]) / 16.0; //in degrees C
		
		for(i=0; i<6; i+=2) {
			gyro[i/2] = 4.375/1000.0*combineRegisters(readBuffer[3+i],readBuffer[2+i]);  //in degrees per second
			accel[i/2] = 0.061/1000.0*combineRegisters(readBuffer[9+i],readBuffer[8+i]); //in multiples of g
		}
		accel[3] = sqrt(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]); //magnitude of accelerometer
		
		printf("Temp: %fC\n",temperature);
		printf("gyro(dps): x=%0.3f y=%0.3f z=%0.3f\n",gyro[0],gyro[1],gyro[2]);
		printf("accel(g) : x=%0.3f y=%0.3f z=%0.3f mag=%0.3f\n",accel[0],accel[1],accel[2],accel[3]);
		
		fprintf(gyro_data,"%f\n",gyro[2]);
		free(readBuffer);
		fclose(gyro_data);
		nanosleep(&time_des,NULL);
		
		/*
		printf("Temp: %02x%02x\n"
			"Gyro: x=%02x%02x y=%02x%02x z=%02x%02x\n"
			"Accl: x=%02x%02x y=%02x%02x z=%02x%02x\n\n",
			readBuffer[1],readBuffer[0],
			readBuffer[3],readBuffer[2],readBuffer[5],readBuffer[4],
				readBuffer[7],readBuffer[6],
			readBuffer[9],readBuffer[8],readBuffer[11],readBuffer[10],
				readBuffer[13],readBuffer[12]);
		*/
	}
	close(file_i2c);
	return 0;
}

unsigned char* readRegisters(int fromAddr, int toAddr, int file_i2c) {
	writeValue(fromAddr,file_i2c);
	int length = toAddr - fromAddr + 1;
	unsigned char *data = (char*)malloc(length * sizeof(unsigned char));
	if(read(file_i2c, data, length) != length) {
		printf("Failed to read in data\n");
		return NULL;
	}
	return data;
}

int writeValue(unsigned char value, int file_i2c) {
	char writeBuffer[1];
	writeBuffer[0] = value;
	if (write(file_i2c, writeBuffer, 1) != 1) {
		printf("Failed to reset the read address\n");
		return 1;
	}
	return 0;
}

short combineRegisters(unsigned char msB, unsigned char lsB) {
	return ((short)msB<<8) | lsB;
}
