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

//#define MINSERVOPWM 143 //  0.02/4096 = 4.9us, 700us / 4.9us = 142.8571
//#define MAXSERVOPWM 408 //  0.02/4096 = 4.9us, 2000us / 4.9us = 408.16327

//265 factors: 1, 5, 53, 265

//#define MINSERVOPWM 216 //  0.02/4096 = 4.9us, 1060us / 4.9us = 216.3265
//#define MAXSERVOPWM 379 //  0.02/4096 = 4.9us, 1860us / 4.9us = 379.5918

#define MINSERVOPWM 173 //  0.02/4096 = 4.9us, 850us / 4.9us = 173.46939
#define MAXSERVOPWM 408 //  0.02/4096 = 4.9us, 2000us / 4.9us = 408.16327

class MOTOR
{
public:
	MOTOR(PCA9685 *, int); //constructor
	int getChannel();
	int getAjustedMIN();
	int getAjustedMAX();
	int getAjustedRange();
	int getRange();
	void setAjustedMIN(uint8_t);
	void setAjustedMAX(uint8_t);
	void armESC();
	void armESCWithList(std::vector<MOTOR>);
	void calibrate();
	void setSpeedPercent(uint8_t);
	void setSpeed(uint16_t);
private:
	PCA9685 *CHIP;
	int CHANNEL;
	int AJUSTEDMIN;
	int AJUSTEDMAX;
};

#endif

