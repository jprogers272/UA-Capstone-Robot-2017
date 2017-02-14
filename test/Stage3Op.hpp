#ifndef STAGE3OP_HPP
#define STAGE3OP_HPP

#include "gpio.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

/***************************************************
 * Stage3OP.hpp - An IEEE SECON 2017 Header File   *
 * Author(s): Brandon Fikes and Bryant Hall        *
 * Intent: Drive the stepper motor for Stage 3 by  *
 * taking in 5 integer values corresponding to the *
 * codes obtained from Stage 1 of the competition  *
 **************************************************/

void Stage3Op(int c1, int c2, int c3, int c4, int c5);

#endif

