/*
 *   Title: GCTRL.h
 *   Description: GCTRL.h
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

#ifndef GCTRL_H_
#define GCTRL_H_

#include <inttypes.h>
#include <string.h>
#include "MOTOR.h"

class GCTRL
{
public:
	std::vector<PCA9685> PCA9685Chip;
	std::vector<MOTOR> motorList;

	GCTRL(); //constructor
	//GCTRL(std::string); //constructor
	GCTRL(std::string, bool); //constructor

	void PCA9685Setup(uint8_t, uint8_t, uint16_t); // ( Bus, address ) defaults chipnumber to 1
	void addMotor(uint8_t, uint8_t, uint8_t, uint8_t, std::string, int8_t); // (chipindex, channel, AjustedMin, AjustedMax, MotorName, direction)
	void addMotor(uint8_t, uint8_t, uint8_t, uint8_t, std::string); // (chipindex, channel, AjustedMin, AjustedMax, MotorName)
	void addMotor(uint8_t, uint8_t, uint8_t, uint8_t); // (chipindex, channel, AjustedMin, AjustedMax)
	void calibrate();
	void arm();
	void isvtail(bool);
	void invertSetMotorDirection(bool);
	
	/*MOTOR* getMotor(std::string)
	MOTOR* getMotor(uint16_t)*/
	
	void motorAjustValueIndividual(MOTOR*, int16_t);
	void motorAjustValueGroup(std::vector<MOTOR>*, int16_t);
	void motorAjustPercentIndividual(MOTOR*, int8_t);
	void motorAjustPercentGroup(std::vector<MOTOR>*, int8_t);

private:
	bool vtail, CW_CCW_inverted;
	std::string droneType;
};

#endif

