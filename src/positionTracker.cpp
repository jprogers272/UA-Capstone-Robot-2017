//Author(s): John Rogers

#include "positionTracker.hpp"
#include "robotDefinitions.hpp"
#include <cmath>
#include <cstddef>
#include <iostream>

using namespace std;

PositionTracker::PositionTracker(void) {
	int i;
	for (i=0; i<3; i++) {
		angle[i] = 0.0;
		velocity[i] = 0.0;
		position[i] = 0.0;
		imu_averages[i] = 0.0;
		imu_averages[i+3] = 0.0;
	}
	time_prev = 0;
}

void PositionTracker::calculateAnglesOnly(float dps_x, float dps_y, float dps_z, long time_cur) {
	int i;
	float dps_data[3] = {dps_x,dps_y,dps_z};
	float time_elapsed = (float)(time_cur - time_prev) / 1000.0;
	for (i=0; i<3; i++) {
		angle[i] += ((dps_data[i] - imu_averages[i]) * time_elapsed);
	}
	time_prev = time_cur;
}

//note that position tracking doesn't work as of now; more filtering would be required
void PositionTracker::calculateAll(float *imu_data, long time_cur) {
	int i;
	float **robotAcclMatrix = new float*[3];
	float **fieldCentricAcclMatrix = new float*[3];
	float time_elapsed = (float)(time_cur - time_prev) / 1000.0;
	cout << "time elapsed is " << time_elapsed << endl;
	//cout << "allocated rows of accl matrices" << endl;
	for (i=0; i<3; i++) {
		robotAcclMatrix[i] = new float;
		fieldCentricAcclMatrix[i] = new float;
		angle[i] += ((imu_data[i] - imu_averages[i]) * time_elapsed);
		//cout << "angular velocity" << i << " is " << imu_data[i] << " and the average is " << imu_averages[i] << endl;
		//cout << "and the angle is " << angle[i] << endl;
		robotAcclMatrix[i][0] = imu_data[i+3] - imu_averages[i+3];
		//cout << "accl " << i << " is " << imu_data[i+3] << " and the average is " << imu_averages[i+3] << endl;
	}
	float **xRotMatrix = calculateXRotMatrix();
	float **yRotMatrix = calculateYRotMatrix();
	float **zRotMatrix = calculateZRotMatrix();
	
	//A_f = R_z * R_y * R_x * A_r
	fieldCentricAcclMatrix = multiplyMatrices(xRotMatrix,robotAcclMatrix,3,3,3,1);
	//cout << "multiplied x rotation matrix and accl matrix" << endl;
	fieldCentricAcclMatrix = multiplyMatrices(yRotMatrix,fieldCentricAcclMatrix,3,3,3,1);
	//cout << "multiplied y rotation matrix and accl matrix" << endl;
	fieldCentricAcclMatrix = multiplyMatrices(zRotMatrix,fieldCentricAcclMatrix,3,3,3,1);
	//cout << "multiplied z rotation matrix and accl matrix" << endl;
	
	for (i=0; i<3; i++) {
		//convert acceleration from g to m/s^2, then integrate twice
		velocity[i] += fieldCentricAcclMatrix[i][0] * 9.81;
		position[i] += velocity[i];
	}
	//cout << "calculated velocity and position vectors" << endl;
	
	deleteMatrix(robotAcclMatrix,3);
	//cout << "deleted robot accl matrix" << endl;
	deleteMatrix(fieldCentricAcclMatrix,3);
	//cout << "deleted field centric accl matrix" << endl;
	deleteMatrix(xRotMatrix,3);
	//cout << "deleted robot x rotation matrix" << endl;
	deleteMatrix(yRotMatrix,3);
	//cout << "deleted robot y rotation matrix" << endl;
	deleteMatrix(zRotMatrix,3);
	//cout << "deleted robot z rotation matrix" << endl;
	
	time_prev = time_cur;
}

void PositionTracker::calculateAll(float gyroX, float gyroY, float gyroZ, float acclX, float acclY, float acclZ, long time_cur) {
	float imu_data[6] = {gyroX,gyroY,gyroZ,acclX,acclY,acclZ};
	calculateAll(imu_data,time_cur);
}

void PositionTracker::setTime(long time) {
	time_prev = time;
}

float ** PositionTracker::multiplyMatrices(float **matrixA, float **matrixB, int rowsA, int columnsA, int rowsB, int columnsB) {
	if (columnsA != rowsB) {
		return NULL;
	}
	int i, j, k;
	float **matrixResult = new float*[rowsA];
	for (i = 0; i < rowsA; i++) {
		matrixResult[i] = new float[columnsB];
		for (j = 0; j < columnsB; j++) {
			matrixResult[i][j] = 0.0;
			for (k = 0; k < columnsA; k++) {
				matrixResult[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}
	return matrixResult;
}

float ** PositionTracker::calculateXRotMatrix(void) {
	float **rotMatrix = new float*[3];
	int i;
	for (i=0; i<3; i++) {
		rotMatrix[i] = new float[3];
	}
	
	float cosine_component = cos(angle[0] * PI / 180.0);
	float sine_component = sin(angle[0] * PI / 180.0);
	
	rotMatrix[0][0] = 1.0;
	rotMatrix[0][1] = 0.0;
	rotMatrix[0][2] = 0.0;
	rotMatrix[1][0] = 0.0;
	rotMatrix[1][1] = cosine_component;
	rotMatrix[1][2] = sine_component * -1.0;
	rotMatrix[2][0] = 0.0;
	rotMatrix[2][1] = sine_component;
	rotMatrix[2][2] = cosine_component;
	
	return rotMatrix;
}

float ** PositionTracker::calculateYRotMatrix(void) {
	float **rotMatrix = new float*[3];
	int i;
	for (i=0; i<3; i++) {
		rotMatrix[i] = new float[3];
	}
	
	float cosine_component = cos(angle[1] * PI / 180.0);
	float sine_component = sin(angle[1] * PI / 180.0);
	
	rotMatrix[0][0] = cosine_component;
	rotMatrix[0][1] = 0.0;
	rotMatrix[0][2] = sine_component;
	rotMatrix[1][0] = 0.0;
	rotMatrix[1][1] = 1.0;
	rotMatrix[1][2] = 0.0;
	rotMatrix[2][0] = sine_component * -1.0;
	rotMatrix[2][1] = 0.0;
	rotMatrix[2][2] = cosine_component;
	
	return rotMatrix;
}

float ** PositionTracker::calculateZRotMatrix(void) {
	float **rotMatrix = new float*[3];
	int i;
	for (i=0; i<3; i++) {
		rotMatrix[i] = new float[3];
	}
	
	float cosine_component = cos(angle[2] * PI / 180.0);
	float sine_component = sin(angle[2] * PI / 180.0);
	
	rotMatrix[0][0] = cosine_component;
	rotMatrix[0][1] = sine_component * -1.0;
	rotMatrix[0][2] = 0.0;
	rotMatrix[1][0] = sine_component;
	rotMatrix[1][1] = cosine_component;
	rotMatrix[1][2] = 0.0;
	rotMatrix[2][0] = 0.0;
	rotMatrix[2][1] = 0.0;
	rotMatrix[2][2] = 1.0;
	
	return rotMatrix;
}

void PositionTracker::deleteMatrix(float **matrix, int rows) {
	int i;
	for (i=0; i<3; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

float * PositionTracker::getAllData(void) {
	float *allData = new float[9];
	int i;
	for (i=0; i<3; i++) {
		allData[i] = angle[i];
		allData[i+3] = velocity[i];
		allData[i+6] = position[i];
	}
	return allData;
}

float PositionTracker::getValue(int type, int direction) {
	if (type == ANGLE) {
		return getAngle(direction);
	}
	else if (type == VELOCITY) {
		return getVelocity(direction);
	}
	else if (type == POSITION) {
		return getPosition(direction);
	}
	else {
		return 0.0;
	}
}

float PositionTracker::getAngle(int direction) {
	return angle[direction];
}

float PositionTracker::getVelocity(int direction) {
	return velocity[direction];
}

float PositionTracker::getPosition(int direction) {
	return position[direction];
}

void PositionTracker::setAverages(float *averages) {
	int i;
	for (i=0; i<6; i++) {
		imu_averages[i] = averages[i];
	}
}
 
void PositionTracker::setGyroAverages(float gyroAverageX, float gyroAverageY, float gyroAverageZ) {
	imu_averages[0] = gyroAverageX;
	imu_averages[1] = gyroAverageY;
	imu_averages[2] = gyroAverageZ;
}
