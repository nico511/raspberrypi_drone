/*
 *   Title: motor.h
 *   Description: Moter.cpp function definitions
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <vector>
#include "PCA9685.h"

#define MINSERVOPWM 143 //  0.02/4096 = 4.9us, 700us / 4.9us = 142.8571
#define MAXSERVOPWM 408 //  0.02/4096 = 4.9us, 2000us / 4.9us = 408.16327

//265 factors: 1, 5, 53, 265

class MOTOR
{
public:
	MOTOR(PCA9685 *, int); //constructor
	int getChannel();
	void armESC();
	void armESCWithList(std::vector<MOTOR>);
	void calibrate_ARM();
	void setSpeedPercent(uint8_t);
	void setSpeed(uint16_t);
private:
	PCA9685 *CHIP;
	int CHANNEL;
	
};

#endif

