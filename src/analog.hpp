//Author(s): John Rogers

#ifndef ANALOG_HPP
#define ANALOG_HPP

#define ADC_FILE_PREFIX "/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc/iio:device0/in_voltage"

int readADC_mv(int);
int readADC_raw(int);
int convertAnalog_raw(int);

#endif