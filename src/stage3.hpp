//Author(s): Brandon Fikes

#ifndef STAGE3_HPP
#define STAGE3_HPP

#include "robotDefinitions.hpp"

class Stage3{
	//members:
	public:
		int rotate1; //first rotation loop variable
		int rotate2; //second rotation loop variable
		int rotate3; //third rotation loop variable
		int rotate4; //fourth rotation loop variable
		int rotate5; //fifth rotation loop variable
		int currentCodeValue;
		int waiting; //wait variable for use in pulsing stepper
		void calculateRotations(int*);
};

#endif
