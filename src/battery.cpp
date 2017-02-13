#include "battery.hpp"
#include "analog.hpp"

float getBatteryVoltage(void) {
	float vbat = VBAT_MULTIPLIER * readADC_mv(2) / 1000.0;
	return vbat;
}
