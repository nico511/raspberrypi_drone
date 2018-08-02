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
	AJUSTEDMIN = MINSERVOPWM;
	AJUSTEDMAX = MAXSERVOPWM;
}

/*   getChannel()
*       returns Channel number on PCA9685 chip (1 - 16)
*/
int MOTOR::getChannel()
{
	return CHANNEL;
}

/*   getAjustedMIN()
*       returns AJUSTMIN value
*/
int MOTOR::getAjustedMIN()
{
        return AJUSTEDMIN;
}

/*   getAjustedMAX()
*       returns AJUSTMAX value
*/
int MOTOR::getAjustedMAX()
{
        return AJUSTEDMAX;
}

/*   getAjustedRange()
*       returns Ajusted Range value
*/
int MOTOR::getAjustedRange()
{
	return AJUSTEDMAX - AJUSTEDMIN;
}

/*   getRange()
*       returns Range value
*/
int MOTOR::getRange()
{
	return MAXSERVOPWM - MINSERVOPWM;
}

/*   setAjustedMIN()
*       Sets AJUSTEDMIN to MINSERVOPWM + 'a' so it is 'a' more than MINSERVOPWM
*/
void MOTOR::setAjustedMIN(uint8_t a)
{
	AJUSTEDMIN = MINSERVOPWM + a;
}

/*   setAjustedMAX()
*       Sets AJUSTEDMAX to MAXSERVOPWM - 'a' so it is 'a' less than MAXSERVOPWM
*/
void MOTOR::setAjustedMAX(uint8_t a)
{
        AJUSTEDMAX = MAXSERVOPWM - a;
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
	wait_ms(1000);

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

        CHIP->setPWM(getChannel(), MINSERVOPWM);
        for( MOTOR m : motors )
                CHIP->setPWM(m.getChannel(), MINSERVOPWM);

	std::printf("Set %d ESCs to MIN\nArming complete\n", s);

	return;
}

/*   calibrate()
*       Calabrates ESC
*/
void MOTOR::calibrate()
{
	CHIP->setPWM(CHANNEL, 0);
        std::printf("Disconnect the battery and press enter");
        std::getchar();
	CHIP->setPWM(CHANNEL, MAXSERVOPWM);
	std::printf("Connect the Battery. After the falling tone, press enter");
        std::getchar();
	CHIP->setPWM(CHANNEL, MINSERVOPWM);
	wait_ms(1000);
	return;
}

/*   SetSpeedPercent(uint8_t)
*       Sets motor speed from values between 0 and 100
*/

void MOTOR::setSpeedPercent(uint8_t percent)
{
	float inc = (AJUSTEDMAX - AJUSTEDMIN) / 100.0;

	if(percent <= 100)
		CHIP->setPWM(getChannel(), AJUSTEDMIN + (int) (percent * inc));
	else
		printf("%d is outside of range 0 - 100", percent);
}

/*   SetSpeed(uint16_t)
*       Sets motor speed from values between 0 and AJUSTEDMAX - AJUSTEDMIN
*/
void MOTOR::setSpeed(uint16_t speed)
{
	if(speed <= (AJUSTEDMAX - AJUSTEDMIN))
		CHIP->setPWM(getChannel(), AJUSTEDMIN + speed);
	else
                printf("%d is outside of range 0 - %d\n", speed, (AJUSTEDMAX - AJUSTEDMIN));
}
