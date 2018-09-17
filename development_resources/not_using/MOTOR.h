/*
 *   Title: motor.h
 *   Description: Moter.cpp function definitions
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <chrono>
#include <thread>
#include <vector>
#include "PCA9685.h"

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
	void setAjustedMIN(uint8_t); // (ajustedAValue)
	void setAjustedMAX(uint8_t); // (ajustedAValue)
	void armESC();
	void armESCWithList(std::vector<MOTOR>); // (List of motors)
	void calibrate();
	void calibrateESCWithList(std::vector<MOTOR>);
	void setSpeedPercent(uint8_t);
	void setSpeed(uint16_t);

	//Static functions

	static void setSpeedPercent(PCA9685 *, uint8_t, uint8_t, uint8_t, uint8_t); // (chip, channel, AjustedMin, AjustedMax, speedPercent)
	static void setSpeed(PCA9685 *, uint8_t, uint8_t, uint8_t, uint16_t); // (chip, channel, AjustedMin, AjustedMax, speedPercent)
	static void armESC(PCA9685 *, uint8_t); // (chip, channel);
	static void armESCFromChannels(PCA9685 *, uint8_t, uint8_t); // (chip, channelBegin, channelEnd)
	static void calibrateDebug(PCA9685 *, uint8_t); // (chip, channel);
	static void calibrate(PCA9685 *, uint8_t); // (chip, channel);
	static void calibrateFromChannels(PCA9685 *, uint8_t, uint8_t); // (chip, channelBegin, channelEnd)

private:
	PCA9685 *CHIP;
	int CHANNEL;
	int AJUSTEDMIN;
	int AJUSTEDMAX;
};

#endif

