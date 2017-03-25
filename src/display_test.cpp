//Author(s): William Khan

#include <iostream>
#include <sstream>
#include <time.h>
#include "timing.hpp"
#include "display.hpp"
#include "i2cbus.hpp"

using namespace std;

int main()
{
	I2Cbus i2c_bus(2);
	Display display(&i2c_bus);
	ostringstream s;
	s << 1 << 2 << 3 << 4 << 5 ;
	display.clearDisplay();
	display.writeDisplay();
	display.writeCenter(s.str(), 0);
	display.writeDisplay();
	robotWait(2,0);
}

