#include "robot.hpp"

int main(void) {
	Robot RT-R2();	
	while (1) {
		if (RT-R2.robotLogic() == 1)
			break;
		RT-R2.setOutputs();
	}
}