/*
 *   Title: motor.h
 *   Description: Moter.cpp function definitions
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "PCA9685.h"
#include <inttypes.h>

class MOTOR
{
public:
	MOTOR(PCA9685 *, int); //constructor
	void init();
	void init(uint8_t); 
private:
	PCA9685 *CHIP;
	int CHANNEL;
	
};

#endif

