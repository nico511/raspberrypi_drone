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

void MOTOR::armESCWithList(std::vector<MOTOR> motors)
{
	size_t s = motors.size() + 1;

	CHIP->setPWM(getChannel(), 0);
	for( MOTOR m : motors )
		CHIP->setPWM(m.getChannel(), 0);

	std::printf("Set %d ESCs to 0\n", s);
	wait_ms(1000);

	CHIP->setPWM(getChannel(), MAXSERVOPWM);
        for( MOTOR m : motors )
                CHIP->setPWM(m.getChannel(), MAXSERVOPWM);

	std::printf("Set %d ESCs to MAX\n", s);
        wait_ms(1000);

        CHIP->setPWM(getChannel(), MAXSERVOPWM);
        for( MOTOR m : motors )
                CHIP->setPWM(m.getChannel(), MAXSERVOPWM);

	std::printf("Set %d ESCs to MIN\nArming complete\n", s);

	return;
}

void MOTOR::calibrate()
{
	return;
}
