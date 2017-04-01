//Author(s): Brandon Fikes

#include "stage3.hpp"
#include "gpio.hpp"
#include "robotDefinitions.hpp"

//setRotations
void Stage3::calculateRotations(int* code){
	rotate1 = code[0];
	rotate2 = code[1];
	rotate3 = code[2];
	rotate4 = code[3];
	rotate5 = code[4];
}
