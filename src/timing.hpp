#ifndef TIMING_HPP
#define TIMING_HPP

#include <ctime>

#define PRECISION_S 0
#define PRECISION_MS 1
#define PRECISION_US 2
#define PRECISION_NS 3

class RobotTimer {
	//members
	private:
		struct timespec time_start;
		struct timespec time_cur;
		int precision;
	
	//member functions
	public:
		void start(void);
		long getTimeElapsed(int);
};

void robotWait(int,int);

#endif
