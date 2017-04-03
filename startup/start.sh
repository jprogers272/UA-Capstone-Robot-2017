#!/bin/sh

/home/capstone/startup/setup.sh
nohup /home/capstone/bin/robotMain & > /home/capstone/startup/robot.out
