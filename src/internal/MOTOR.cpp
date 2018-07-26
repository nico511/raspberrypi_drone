/*
 *   Title: motor.h
 *   Description: Moter.cpp function definitions
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

#include "MOTOR.h"
#include "functions.h"

MOTOR::MOTOR(PCA9685 *chip, int channel)
{
	CHIP = chip;
	CHANNEL = channel;
}

int MOTOR::getChannel()
{
	return CHANNEL;
}

void MOTOR::armESC()
{
	CHIP->setPWM(CHANNEL, 0);
	wait_ms(1000);
	CHIP->setPWM(CHANNEL, MAXSERVOPWM);
	wait_ms(1000);	
	CHIP->setPWM(CHANNEL, MINSERVOPWM);
	
	return;
}

void MOTOR::armESCList(MOTOR m[])
{
	size_t s = sizeof(m) / sizeof(m[0]);
	std::printf("%d\n", s);
	return;
}

void MOTOR::calibrate()
{
	return;
}
