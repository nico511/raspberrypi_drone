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

/*   getChannel()
*       returns Channel number on PCA9685 chip (1 - 16)
*/
int MOTOR::getChannel()
{
	return CHANNEL;
}

/*   armESC()
*       Arms ESC on initalized channel
*/
void MOTOR::armESC()
{
	CHIP->setPWM(CHANNEL, 0);
	wait_ms(1000);
	CHIP->setPWM(CHANNEL, MAXSERVOPWM);
	wait_ms(1000);
	CHIP->setPWM(CHANNEL, MINSERVOPWM);

	return;
}

/*   armESCWithList(std::vector<MOTOR>)
*       Arms ESC with self and other selected motors
*       Example:
*          motor1.armESCWithList({motor2, motor3, motor4, ...});  <-- any amount of motors can be added
*/
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

/*   calibrate()
*       Calabrates ESC
*/
void MOTOR::calibrate_ARM()
{
	CHIP->setPWM(CHANNEL, 0);
        std::printf("Disconnect the battery and press enter");
        std::getchar();
	CHIP->setPWM(CHANNEL, MAXSERVOPWM);
	std::printf("After the falling tone, press enter");
        std::getchar();
	CHIP->setPWM(CHANNEL, MINSERVOPWM);
	wait_ms(12000);
	std::printf("Arming...\n");
	CHIP->setPWM(CHANNEL, 0);
	wait_ms(2000);
	CHIP->setPWM(CHANNEL, MINSERVOPWM);
	wait_ms(1000);
	return;
}

/*   SetSpeedPercent(uint8_t)
*       Sets motor speed from values between 0 and 100
*/

void MOTOR::setSpeedPercent(uint8_t percent)
{
	float inc = (MAXSERVOPWM - MINSERVOPWM) / 100.0;

	if(percent <= 100)
		CHIP->setPWM(getChannel(), MINSERVOPWM + (int) (percent * inc));
	else
		printf("%d is outside of range 0 - 100", percent);
}

/*   SetSpeed(uint16_t)
*       Sets motor speed from values between 0 and 265
*/
void MOTOR::setSpeed(uint16_t speed)
{
	if(speed <= 265)
		CHIP->setPWM(getChannel(), MINSERVOPWM + speed);
	else
                printf("%d is outside of range 0 - 265", speed);
}
