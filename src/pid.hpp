#ifndef PID_HPP
#define PID_HPP

#include <string>

class PIcontroller {
	//members
	protected:
		float setpoint;
		float gainP;
		float gainI;
		float limit_upper;
		float limit_lower;
		float plant_value;
		float error;
		float error_prev;
		float integral;
		float output;
		int time_prev;
		
		std::string fileCSV;
	
	//member functions
	public:
		PIcontroller(float,float,float,float,float);
		virtual void calculateOutput(float,int);
		void setSetpoint(float);
		float getOutput(void);
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
		PIDcontroller(float,float,float,float,float,float);
		void calculateOutput(float,float);
};

#endif
