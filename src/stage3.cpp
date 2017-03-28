//Author(s): Brandon Fikes

#include "stage3.hpp"
#include "gpio.hpp"
#include "robotDefinitions.hpp"

//setRotations
void Stage3::calculateRotations(int* code){
	rotate1 = code[0]*200;
	rotate2 = code[1]*200;
	rotate3 = code[2]*200;
	rotate4 = code[3]*200;
	rotate5 = code[4]*200;
}