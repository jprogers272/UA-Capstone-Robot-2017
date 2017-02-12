#ifndef ANGLECONTROL_HPP
#define ANGLECONTROL_HPP

#include "pid.hpp"

class AngleControl : public PIcontroller {
	//members
	private:
		float angle;
		float average_dps;
		int time_prev; //in milliseconds
		
	//member functions
	public:
		AngleControl();
		void calculateRotation(float,int);
		void setAverage(float);
		
	private:
		void calculateAngle(float,int);
};

#endif
