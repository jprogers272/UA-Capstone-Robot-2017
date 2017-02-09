#include "imu.hpp"
#include "i2cbus.hpp"
#include <iostream>
#include <cmath>

IMU::IMU(I2Cbus *sensor_bus) : sensor_bus(sensor_bus) {
	sensor_bus->setDevice(imu_address);
	//set accelerometer and gyro to on
	sensor_bus->writeRegister(0x10,0x11);
	sensor_bus->writeRegister(0x11,0x11);
}

float IMU::getTemp(void) {
	sensor_bus->setDevice(imu_address);
	
	unsigned char *readBuffer;
	readBuffer = sensor_bus->readRegisters(0x20,0x21);
	delete(readBuffer);
	
	return 25.0 + combineRegisters(readBuffer[1],readBuffer[0]) / 16.0;
}

float IMU::getGyroX(void) {
	sensor_bus->setDevice(imu_address);
	
	unsigned char *readBuffer;
	readBuffer = sensor_bus->readRegisters(0x22,0x23);
	delete(readBuffer);
	
	return 4.375/1000.0*combineRegisters(readBuffer[1],readBuffer[0]);
}

float IMU::getGyroY(void) {
	sensor_bus->setDevice(imu_address);
	
	unsigned char *readBuffer;
	readBuffer = sensor_bus->readRegisters(0x24,0x25);
	delete(readBuffer);
	
	return 4.375/1000.0*combineRegisters(readBuffer[1],readBuffer[0]);
}

float IMU::getGyroZ(void) {
	sensor_bus->setDevice(imu_address);	
	
	unsigned char *readBuffer;
	readBuffer = sensor_bus->readRegisters(0x26,0x27);
	delete(readBuffer);
	
	return 4.375/1000.0*combineRegisters(readBuffer[1],readBuffer[0]);
}

float IMU::getAcclX(void) {
	sensor_bus->setDevice(imu_address);
	
	unsigned char *readBuffer;
	readBuffer = sensor_bus->readRegisters(0x28,0x29);
	delete(readBuffer);
	
	return 0.061/1000.0*combineRegisters(readBuffer[1],readBuffer[0]);
}

float IMU::getAcclY(void) {
	sensor_bus->setDevice(imu_address);
	
	unsigned char *readBuffer;
	readBuffer = sensor_bus->readRegisters(0x2a,0x2b);
	delete(readBuffer);
	
	return 0.061/1000.0*combineRegisters(readBuffer[1],readBuffer[0]);
}

float IMU::getAcclZ(void) {
	sensor_bus->setDevice(imu_address);
	
	unsigned char *readBuffer;
	readBuffer = sensor_bus->readRegisters(0x2c,0x2d);
	delete(readBuffer);
	
	return 0.061/1000.0*combineRegisters(readBuffer[1],readBuffer[0]);
}

//returns magnitude of acceleration vector
float IMU::getAcclMag(void) {
	sensor_bus->setDevice(imu_address);
	float accel[3];
	accel[0] = this->getAcclX();
	accel[1] = this->getAcclY();
	accel[2] = this->getAcclZ();
	return sqrt(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);
}

//returns a float array, whoever calls this function, must delete returned pointer after using it
float *IMU::getDataAll(void) {
	sensor_bus->setDevice(imu_address);
	
	unsigned char *readBuffer;
	readBuffer = sensor_bus->readRegisters(0x20,0x2d);
	delete(readBuffer);
	
	float temperature;
	float gyro[3];
	float accel[3];
	
	temperature = 25.0 + combineRegisters(readBuffer[1],readBuffer[0]) / 16.0; //in degrees C
	
	int i;
	for(i=0; i<6; i+=2) {
		gyro[i/2] = 4.375/1000.0*combineRegisters(readBuffer[3+i],readBuffer[2+i]);  //in degrees per second
		accel[i/2] = 0.061/1000.0*combineRegisters(readBuffer[9+i],readBuffer[8+i]); //in multiples of g
	}
	accel[3] = sqrt(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]); //magnitude of accelerometer
	
	float *return_values = new float[8];
	return_values[0] = temperature;
	return_values[1] = gyro[0];
	return_values[2] = gyro[1];
	return_values[3] = gyro[2];
	return_values[4] = accel[0];
	return_values[5] = accel[1];
	return_values[6] = accel[2];
	return_values[7] = accel[3];
	
	return return_values;
}

void IMU::printData(void) {
	float *values = getDataAll();
	
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cout << "Temp: " << values[0] << "C\n";
	std::cout << "gyro(dps): x=" << values[1]
				<< " y=" << values[2]
				<< " z=" << values[3]
				<< '\n';
	std::cout << "accel(g) : x=" << values[4]
				<< " y=" << values[5]
				<< " z=" << values[6]
				<< " mag=" << values[7]
				<< '\n';
	delete(values);
}

short IMU::combineRegisters(unsigned char msB, unsigned char lsB) {
	return ((short)msB<<8) | lsB;
}