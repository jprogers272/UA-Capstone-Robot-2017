//Author(s): John Rogers

#ifndef ANGLECONTROL_HPP
#define ANGLECONTROL_HPP

#include "pid.hpp"

class AngleControl : public PIDcontroller {
	//members
	private:
		float angle;
		float average_dps;
		
	//member functions
	public:
		AngleControl(float,float,float);
		float calculateRotation(float,int);
		void setAverage(float);
		float getAngle(void);
		void setAngle(float);
		
	private:
		void calculateAngle(float,int);
};

#endif
