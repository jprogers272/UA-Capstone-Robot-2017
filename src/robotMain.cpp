#include "robot.hpp"

int main(void) {
	Robot RTR2;	
	while (1) {
		if (RTR2.robotLogic() == 1)
			break;
		RTR2.setOutputs();
	}
}