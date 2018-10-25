/*
 *   Title: GCTRL.cpp
 *   Description: GCTRL.h function definitions
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

//#include <cmath>
#include <iostream>
#include "GCTRL.h"

GCTRL::GCTRL()
{
	CW_CCW_inverted = false;
	vtail = false;
	droneType = "unknown";
}

//GCTRL::GCTRL(std::string type)

GCTRL::GCTRL(std::string type, bool invert)
{
	CW_CCW_inverted = invert;
	
	//add to lower function
	droneType = type;
	
	if(droneType == "quadp")
	{
		
	}
	else if(droneType == "quadx")
	{
		
	}
	else if(droneType == "vtail")
	{
		vtail = true;
		
	}
	else
	{
		std::cout << "unknown drone type\nManual setup required.\n";
	}
}

void GCTRL::PCA9685Setup(uint8_t bus, uint8_t address, uint16_t frequency)
{
	//PCA9685Chip.insert(PCA9685Chip.end(), PCA9685(bus, address)); //add +1 after vector.begin() to insert in vector[1]
	PCA9685Chip.push_back( PCA9685(bus, address) );
	PCA9685Chip[PCA9685Chip.size() - 1].setPWMFreq(frequency);
}

void GCTRL::addMotor(uint8_t chipIndex, uint8_t channel, uint8_t ajustedMin, uint8_t ajustedMax, std::string name, int8_t dir)
{
	//motorList.insert(motorList.end(), MOTOR( &PCA9685Chip.data()[chipIndex], (uint8_t) channel, (uint8_t) ajustedMin, (uint8_t) ajustedMax, (std::string) name));
	motorList.push_back( MOTOR( &PCA9685Chip.data()[chipIndex], (uint8_t) channel, (uint8_t) ajustedMin, (uint8_t) ajustedMax, (std::string) name));
	if(dir == CW)
	{
		motorList[motorList.size() - 1].setDirection(CW);
	}
	else if(dir == CCW)
	{
		motorList[motorList.size() - 1].setDirection(CCW);
	}
	else
	{
		std::cout << "Unkown direction. setting to 0.";
		motorList[motorList.size() - 1].setDirection(0);
	}
}

void GCTRL::addMotor(uint8_t chipIndex, uint8_t channel, uint8_t ajustedMin, uint8_t ajustedMax, std::string name)
{
	//motorList.insert(motorList.end(), MOTOR( &PCA9685Chip.data()[chipIndex], (uint8_t) channel, (uint8_t) ajustedMin, (uint8_t) ajustedMax, (std::string) name));
	motorList.push_back( MOTOR( &PCA9685Chip.data()[chipIndex], (uint8_t) channel, (uint8_t) ajustedMin, (uint8_t) ajustedMax, (std::string) name));

}

void GCTRL::addMotor(uint8_t chipIndex, uint8_t channel, uint8_t ajustedMin, uint8_t ajustedMax)
{
	//motorList.insert(motorList.end(), MOTOR( &PCA9685Chip.data()[chipIndex], (uint8_t) channel, (uint8_t) ajustedMin, (uint8_t) ajustedMax);
	motorList.push_back( MOTOR( &PCA9685Chip.data()[chipIndex], (uint8_t) channel, (uint8_t) ajustedMin, (uint8_t) ajustedMax));

}

void GCTRL::calibrate()
{
	if(motorList.empty())
		std::cout << "No Motors in list\n";
	else
		MOTOR::calibrateESCWithList((std::vector<MOTOR>) motorList);
}

void GCTRL::arm()
{
	if(motorList.empty())
		std::cout << "No Motors in list\n";
	else
		MOTOR::armESCWithList((std::vector<MOTOR>) motorList);
}

void GCTRL::isvtail(bool value)
{
	vtail = value;
}

void GCTRL::invertSetMotorDirection(bool value)
{
	CW_CCW_inverted = value;
}

void GCTRL::motorAjustValueIndividual(MOTOR *m, int16_t ajust)
{
	
}

void GCTRL::motorAjustValueGroup(std::vector<MOTOR> *m, int16_t ajust)
{
	
}

void GCTRL::motorAjustPercentIndividual(MOTOR *m, int8_t ajust)
{
	
}

void GCTRL::motorAjustPercentGroup(std::vector<MOTOR> *m, int8_t ajust)
{
	
}