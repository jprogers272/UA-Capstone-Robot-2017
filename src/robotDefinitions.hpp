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

#define STAGE1_GPIO1 14
#define STAGE1_GPIO2 112
#define STAGE1_GPIO3 7
#define STAGE1_GPIO4 15
#define STAGE1_GPIO5 113
#define STAGE1_GPIO_COM 51
#define STAGE1_ADC 0
#define STAGE1_CHARGING_TIME 10

#define IR1_1_GPIO 8
#define IR1_2_GPIO 76
#define IR1_3_GPIO 27
#define IR1_4_GPIO 47
#define IR2_1_GPIO 78
#define IR2_2_GPIO 86
#define IR2_3_GPIO 69
#define IR2_4_GPIO 66
#define IR4_1_GPIO 87
#define IR4_2_GPIO 10

#define GUN_GPIO 9

#define WHEEL1_DIR 73
#define WHEEL2_DIR 75
#define WHEEL3_DIR 11
#define WHEEL4_DIR 89
#define SLAPPER_DIR 72

#define WHEEL1_PWM PWM2B
#define WHEEL2_PWM PWM2A
#define WHEEL3_PWM PWM1B
#define WHEEL4_PWM PWM1A
#define SLAPPER_PWM PWM0A

#define LOOP_TIME_MS 10

#define PWM_PERIOD 50000

#define PGAIN 0.1
#define IGAIN 0.03
#define DGAIN 0.005

#define VBAT_MULTIPLIER 11.2

#endif
