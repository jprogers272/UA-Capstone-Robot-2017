//Author(s): John Rogers

#ifndef POSITIONTRACKER_HPP
#define POSITIONTRACKER_HPP

#define X 0
#define Y 1
#define Z 2
#define ANGLE 0
#define VELOCITY 1
#define POSITION 2

class PositionTracker {
	//members
	public:
		
	private:
		float angle[3];
		float velocity[3];
		float position[3];
		float imu_averages[6];
		long time_prev;
	
	//member functions
	public:
		PositionTracker(void);
		void calculateAnglesOnly(float,float,float,long);
		void calculateAll(float*,long);
		void calculateAll(float,float,float,float,float,float,long);
		void setTime(long);
		float *getAllData(void);
		float getValue(int,int);
		float getAngle(int);
		float getVelocity(int);
		float getPosition(int);
		void setAverages(float*);
		void setGyroAverages(float,float,float);
		
	private:
		float **multiplyMatrices(float**,float**,int,int,int,int);
		float **calculateXRotMatrix(void);
		float **calculateYRotMatrix(void);
		float **calculateZRotMatrix(void);
		void deleteMatrix(float**,int);
};

#endif
