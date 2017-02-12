#include "timing.hpp"
#include <ctime>

void RobotTimer::start(void) {
	clock_gettime(CLOCK_MONOTONIC, &time_start);
}

long RobotTimer::getTimeElapsed(int precision) {
	clock_gettime(CLOCK_MONOTONIC, &time_cur);
	long time_elapsed;
	
	switch(precision) {
		case PRECISION_S:
			time_elapsed = (long)(time_cur.tv_sec - time_start.tv_sec);
		case PRECISION_MS:
			time_elapsed = (long)(time_cur.tv_sec - time_start.tv_sec) * 1000;
			time_elapsed += (time_cur.tv_nsec - time_start.tv_nsec) / 1000000;
		case PRECISION_US:
			time_elapsed = (long)(time_cur.tv_sec - time_start.tv_sec) * 1000000;
			time_elapsed += (time_cur.tv_nsec - time_start.tv_nsec) / 1000;
		case PRECISION_NS:			
			time_elapsed = (long)(time_cur.tv_sec - time_start.tv_sec) * 1000000000;
			time_elapsed += (time_cur.tv_nsec - time_start.tv_nsec);
		default:
			time_elapsed = 0;
	}
	
	return time_elapsed;
}


void robotWait(int seconds, int milliseconds) {
	struct timespec time_wait;
	time_wait.tv_sec = seconds;
	time_wait.tv_nsec = (long)milliseconds * 1000000;
	
	nanosleep(&time_wait,NULL);
}

