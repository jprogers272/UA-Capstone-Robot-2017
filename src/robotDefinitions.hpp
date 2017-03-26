//Author(s): John Rogers

#ifndef ROBOTDEFINITIONS_HPP
#define ROBOTDEFINITIONS_HPP

#define PI 3.14159

#define TRUE 1
#define FALSE 0

#define GPIO_OUTPUT 0
#define GPIO_INPUT 1

#define STRAIGHT_FORWARD 0
#define STRAIGHT_BACKWARD 1
#define STRAFE_LEFT 2
#define STRAFE_RIGHT 3
#define ROTATE 4
#define STOPPED 5

#define STAGE1_GPIO1 20
#define STAGE1_GPIO2 115
#define STAGE1_GPIO3 14
#define STAGE1_GPIO4 117
#define STAGE1_GPIO5 49
#define STAGE1_GPIO_COM 7
#define STAGE1_ADC 0
#define STAGE1_CHARGING_TIME 10

#define IR1_1_GPIO 66
#define IR1_2_GPIO 69
#define IR1_3_GPIO 27
#define IR1_4_GPIO 47
#define IR2_1_GPIO 87
#define IR2_2_GPIO 48
#define IR2_3_GPIO 77
#define IR2_4_GPIO 76
#define IR4_1_GPIO 15	
#define IR4_2_GPIO 67

#define GUN_GPIO 50

#define WHEEL1_DIR 9
#define WHEEL2_DIR 8
#define WHEEL3_DIR 26
#define WHEEL4_DIR 65
#define SLAPPER_DIR 112

#define WHEEL1_PWM PWM1B
#define WHEEL2_PWM PWM1A
#define WHEEL3_PWM PWM2B
#define WHEEL4_PWM PWM2A
#define SLAPPER_PWM PWM0B

#define LOOP_TIME_MS 10

#define PWM_PERIOD 50000

#define PGAIN 0.1
#define IGAIN 0.03
#define DGAIN 0.005

#define VBAT_MULTIPLIER 11.2

#endif
