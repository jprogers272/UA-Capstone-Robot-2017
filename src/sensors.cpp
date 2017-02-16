#include "sensors.hpp"
#include "battery.hpp"
#include "robotDefinitions.hpp"

Sensors::Sensors(I2Cbus *i2cbus) :
	ir1_1(IR1_1_GPIO),
	ir1_2(IR1_2_GPIO),
	ir1_3(IR1_3_GPIO),
	ir1_4(IR1_4_GPIO),
	ir2_1(IR2_1_GPIO),
	ir2_2(IR2_2_GPIO),
	ir2_3(IR2_3_GPIO),
	ir2_4(IR2_4_GPIO),
	ir4_1(IR4_1_GPIO),
	ir4_2(IR4_2_GPIO),
	imu(i2cbus),
	compass(i2cbus)
{ }

void Sensors::getAllSensors(SensorData *data) {
	data->ir1_1_state = ir1_1.getData();
	data->ir1_2_state = ir1_2.getData();
	//data->ir1_3_state = ir1_3.getData();
	//data->ir1_4_state = ir1_4.getData();
	data->ir2_1_state = ir2_1.getData();
	data->ir2_2_state = ir2_2.getData();
	//data->ir2_3_state = ir2_3.getData();
	//data->ir2_4_state = ir2_4.getData();
	//data->ir4_1_state = ir4_1.getData();
	//data->ir4_2_state = ir4_2.getData();
	data->gyroZ = imu.getGyroZ();
	data->acclY = imu.getAcclY();
	data->compass_angle = compass.getAngleF();
	data->battery_voltage = getBatteryVoltage();
}