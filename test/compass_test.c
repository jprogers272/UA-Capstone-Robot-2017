#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <math.h>

#define ADDR 0x60
#define BUFFER_SIZE 40

unsigned char* readRegisters(int,int,int);
int writeValue(unsigned char,int);
short combineRegisters(unsigned char,unsigned char);

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
	struct timespec time_des;
	time_des.tv_nsec = 10000000; //10 ms
	time_des.tv_sec = 0;
	
	//compass returns data as both single byte 0-255 data, and one word 0-3559 data
	int bearing_byte;
	float bearing_fbyte;
	float bearing_word;
	
	free(readBuffer);
	while(1) {
		readBuffer = readRegisters(0x01,0x03,file_i2c);
		
		bearing_byte = readBuffer[0];
		bearing_fbyte = bearing_byte * 360.0/255.0;
		bearing_word = combineRegisters(readBuffer[1],readBuffer[2]) / 10.0;
		
		printf("byte: %d\n",bearing_byte);
		printf("byte: %f degrees\n",bearing_fbyte);
		printf("word: %f degrees\n",bearing_word);
		
		free(readBuffer);
		nanosleep(time_des);
		
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