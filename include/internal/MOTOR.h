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

#define MINSERVOPWM 143 //  0.02/4096 = 4.9us, 700us / 4.9us = 142.8571
#define MAXSERVOPWM 408 //  0.02/4096 = 4.9us, 2000us / 4.9us = 408.16327

//265 factors: 1, 5, 53, 265

class MOTOR
{
public:
	MOTOR(PCA9685 *, int); //constructor
	int getChannel();
	void armESC();
	void armESCList(MOTOR *);
	void calibrate();
private:
	PCA9685 *CHIP;
	int CHANNEL;
	
};

#endif

