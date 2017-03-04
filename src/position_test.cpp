//Author(s): John Rogers

#include "robotDefinitions.hpp"
#include "i2cbus.hpp"
#include "imu.hpp"
#include "positionTracker.hpp"
#include "timing.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void averageIMU(IMU*,PositionTracker*);

int main (void) {
	I2Cbus i2c_bus(2);
	IMU imu(&i2c_bus);
	PositionTracker position_tracker;
	
	averageIMU(&imu,&position_tracker);
	
	string csv_path("/home/position.csv");
	ofstream csv_file;
	csv_file.open(csv_path.c_str());
	csv_file << "Time(s),dps_x,dps_y,dps_z,angle_x,angle_y,angle_z,accl_x,accl_y,accl_z,vel_x,vel_y,vel_z,pos_x,pos_y,pos_z\n";
	csv_file.close();

	RobotTimer timer;
	timer.start();
	while (1) {
		position_tracker.calculateAll(
			imu.getGyroX(),imu.getGyroY(),imu.getGyroZ(),
			imu.getAcclX(),imu.getAcclY(),imu.getAcclZ(),
			timer.getTimeElapsed(PRECISION_MS));
		
		cout << "Angles(Â): " << 
			position_tracker.getValue(ANGLE,X) << ", " <<
			position_tracker.getValue(ANGLE,Y) << ", " <<
			position_tracker.getValue(ANGLE,Z) << endl;
		cout << "Velocity(m/s): " << 
			position_tracker.getValue(VELOCITY,X) << ", " <<
			position_tracker.getValue(VELOCITY,Y) << ", " <<
			position_tracker.getValue(VELOCITY,Z) << endl;
		cout << "Position(m): " << 
			position_tracker.getValue(POSITION,X) << ", " <<
			position_tracker.getValue(POSITION,Y) << ", " <<
			position_tracker.getValue(POSITION,Z) << endl;
		cout << '\n';	

		csv_file.open(csv_path.c_str(),ios::app);
		csv_file << timer.getTimeElapsed(PRECISION_MS)/1000.0 << ',' <<
			imu.getGyroX() << ',' <<
			imu.getGyroY() << ',' <<
			imu.getGyroZ() << ',' <<
			position_tracker.getValue(ANGLE,X) << ',' <<
			position_tracker.getValue(ANGLE,Y) << ',' <<
			position_tracker.getValue(ANGLE,Z) << ',' <<
			imu.getAcclX() << ',' <<
			imu.getAcclY() << ',' <<
			imu.getAcclZ() << ',' <<
			position_tracker.getValue(VELOCITY,X) << ',' <<
			position_tracker.getValue(VELOCITY,Y) << ',' <<
			position_tracker.getValue(VELOCITY,Z) << ',' <<
			position_tracker.getValue(POSITION,X) << ',' <<
			position_tracker.getValue(POSITION,Y) << ',' <<
			position_tracker.getValue(POSITION,Z) << ',' << endl;
		csv_file.close();

		robotWait(0,10);
	}
}

void averageIMU(IMU *imu, PositionTracker *position_tracker) {
	float imu_averages[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
	int i = 0;
	int j;
	RobotTimer timer;
	timer.start();
	while (i < 50) {
		imu_averages[0] += imu->getGyroX();
		imu_averages[1] += imu->getGyroY();
		imu_averages[2] += imu->getGyroZ();
		imu_averages[3] += imu->getAcclX();
		imu_averages[4] += imu->getAcclY();
		imu_averages[5] += imu->getAcclZ();
		robotWait(0,20);
		i++;
	}
	for (j=0; j<6; j++) {
		imu_averages[j] /= 50.0;
		cout << "average is " << imu_averages[j] << endl;
	}
	position_tracker->setAverages(imu_averages);
}
