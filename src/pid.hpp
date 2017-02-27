//Author(s): John Rogers

#ifndef PID_HPP
#define PID_HPP

#include <string>

class PIDcontroller {
	//members
	protected:
		float setpoint;
		float gainP;
		float gainI;
		float gainD;
		float limit_upper;
		float limit_lower;
		float plant_value;
		float error;
		float error_prev;
		float integral;
		float derivative;
		float output;
		int time_prev;
		
		std::string fileCSV;
	
	//member functions
	public:
		PIDcontroller(float,float,float,float,float,float);
		virtual void calculateOutput(float,int);
		void setSetpoint(float);
		float getOutput(void);
		void openFileCSV(std::string);
		void printFileCSV(void);
		void disableIntegral(void);
		void enableIntegral(float gainI);
		
	private:
		void limitOutput(void);
};

#endif
