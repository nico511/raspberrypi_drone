/*
 *   Title: motor.h
 *   Description: Moter.cpp function definitions
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

#include "MOTOR.h"

MOTOR::MOTOR(PCA9685 *chip, int channel)
{
	CHIP = chip;
	CHANNEL = channel;
}
