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
#include <string>
#include <inttypes.h>
#include "PCA9685.h"

#define DEFAULTMINSERVOPWM 173 //  0.02/4096 = 4.9us, 850us / 4.9us = 173.46939
#define DEFAULTMAXSERVOPWM 408 //  0.02/4096 = 4.9us, 2000us / 4.9us = 408.16327
#define CW 1
#define CCW -1

class MOTOR
{
public:
	MOTOR(PCA9685 *, uint8_t); //constructor (PCA9685, channel)
	MOTOR(PCA9685 *, uint8_t, uint8_t, uint8_t); // (chip, channel, AjustedMin, AjustedMax)
	MOTOR(PCA9685 *, uint8_t, uint8_t, uint8_t, std::string); // (chip, channel, AjustedMin, AjustedMax, name)
	MOTOR(PCA9685 *, uint8_t, uint16_t, uint16_t, uint8_t, uint8_t); // (chip, channel, minservopwm, maxservopwm, AjustedMin, AjustedMax)
	MOTOR(PCA9685 *, uint8_t, uint16_t, uint16_t, uint8_t, uint8_t, std::string);// (chip, channel, minservopwm, maxservopwm, AjustedMin, AjustedMax, name)
	// Channel does not start at 0 becuase PCA9685 library starts with 1
	
	std::string getName();
	uint8_t getChannel();
	PCA9685* getPCA9685();
	uint16_t getAjustedMIN();
	uint16_t getAjustedMAX();
	uint16_t getMINSERVOPWM();
	uint16_t getMAXSERVOPWM();
	uint16_t getAjustedRange();
	uint16_t getRange();
	int8_t getDirection();
	void setDirection(int8_t);
	void setMINSERVOPWM(uint16_t);
	void setMAXSERVOPWM(uint16_t);
	void setAjustedMIN(uint8_t); // (ajustedAValue)
	void setAjustedMAX(uint8_t); // (ajustedAValue)
	void setName(std::string);
	void armESC();
	void calibrate();
	void setSpeedPercent(uint8_t);
	void setSpeed(uint16_t);

	//Static functions

	static void setSpeedPercent(PCA9685 *, uint8_t, uint8_t, uint8_t, uint8_t); // (chip, channel, AjustedMin, AjustedMax, speedPercent)
	static void setSpeedPercent(PCA9685 *, uint8_t, uint16_t, uint16_t, uint8_t, uint8_t, uint8_t); // (chip, channel, minservopwm, maxservopwm, AjustedMin, AjustedMax, speedPercent)
	static void setSpeed(PCA9685 *, uint8_t, uint8_t, uint8_t, uint16_t); // (chip, channel, AjustedMin, AjustedMax, speedPercent)
	static void setSpeed(PCA9685 *, uint8_t, uint16_t, uint16_t, uint8_t, uint8_t, uint16_t); // (chip, channel, minservopwm, maxservopwm, AjustedMin, AjustedMax, speedPercent)
	static void armESC(PCA9685 *, uint8_t); // (chip, channel);
	static void armESC(PCA9685 *, uint8_t, uint16_t); // (chip, channel, minservopwm)
	static void armESCFromChannels(PCA9685 *, uint8_t, uint8_t); // DONOTUSE (chip, channelBegin, channelEnd)
	static void armESCWithList(std::vector<MOTOR>); // (List of motors)
	static void calibrateDebug(PCA9685 *, uint8_t); // (chip, channel);
	static void calibrate(PCA9685 *, uint8_t); // (chip, channel);
	static void calibrate(PCA9685 *, uint8_t, uint16_t, uint16_t);
	static void calibrateFromChannels(PCA9685 *, uint8_t, uint8_t); // DO NOT USE(chip, channelBegin, channelEnd)
	static void calibrateESCWithList(std::vector<MOTOR>);
	

private:
	PCA9685 *CHIP;
	uint8_t CHANNEL;
	uint16_t AJUSTEDMIN;
	uint16_t AJUSTEDMAX;
	uint16_t MINSERVOPWM;
	uint16_t MAXSERVOPWM;
	std::string NAME;
	int8_t DIRECTION;
};

#endif

