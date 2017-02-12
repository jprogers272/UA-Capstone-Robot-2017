#ifndef PID_HPP
#define PID_HPP

#include <string>

class PIcontroller {
	//members
	protected:
		float setpoint;
		float gainP;
		float gainI;
		float plant_value;
		float error;
		float error_prev;
		float integral;
		float output;
		int time_prev;
		float limit_upper;
		float limit_lower;
		
		std::string fileCSV;
	
	//member functions
	public:
		PIcontroller(float,float,float,float,float);
		virtual float calculateOutput(float,float);
		void setSetpoint(float);
		float getOutput(float);
		void openFileCSV(std::string);
		void printFileCSV(void);
		
	protected:
		void limitOutput(void);
};

class PIDcontroller: public PIcontroller {
	//members
	private:
		float gainD;
		float derivative;
		
	//member functions
	public:
		PIDcontroller(float,float,float,float);
		float calculateOutput(float,float);
		void printValues(void);
};

#endif