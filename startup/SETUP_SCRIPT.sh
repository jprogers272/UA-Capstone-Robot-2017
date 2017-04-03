#!/bin/bash

config-pin P9.23 gpio
config-pin P9.25 gpio
config-pin P9.26 gpio
config-pin P9.27 gpio
config-pin P9.41 gpio
config-pin P9.42 gpio

config-pin P8.34 pwm
config-pin P8.36 pwm
config-pin P8.13 pwm
config-pin P8.19 pwm
config-pin P9.29 pwm

#PWM0A - P9.29
echo 1 > /sys/devices/platform/ocp/48300000.epwmss/48300200.pwm/pwm/pwmchip*/export
#PWM1A - P8.36
echo 0 > /sys/devices/platform/ocp/48302000.epwmss/48302200.pwm/pwm/pwmchip*/export
#PWM1B - P8.34
echo 1 > /sys/devices/platform/ocp/48302000.epwmss/48302200.pwm/pwm/pwmchip*/export
#PWM2A - P8.19
echo 0 > /sys/devices/platform/ocp/48304000.epwmss/48304200.pwm/pwm/pwmchip*/export
#PWM2B - P8.13
echo 1 > /sys/devices/platform/ocp/48304000.epwmss/48304200.pwm/pwm/pwmchip*/export
