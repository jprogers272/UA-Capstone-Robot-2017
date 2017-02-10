#ifndef PIDCONTROL_HPP
#define PIDCONTROL_HPP

class PIDcontroller {
	//members
	protected:
		float gainP, gainI;
		float setpoint;
		float error, error_prev;
		float integral;
		float output;
		float time_prev;
		
	private:
		float gainD; //not needed by PI
		float derivative;
	
	//member functions
	public:
		PIDcontroller(float,float,float,float);
		virtual float calculateOutput(float,float);
		void setSetpoint(float);
};

class PIcontroller: public PIDcontroller (
	//member functions
	public:
		PI(float,float,float);
		float calculateOutput(float,float);
);

#endif