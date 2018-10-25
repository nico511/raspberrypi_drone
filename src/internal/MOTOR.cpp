/*
 *   Title: MOTOR.cpp
 *   Description: MOTOR.cpp function definitions
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

#include <chrono>
#include <thread>
#include <iostream>
//#include <cstdio>

#include "MOTOR.h"

MOTOR::MOTOR(PCA9685 *chip, uint8_t channel) // Channel does not start at 0 becuase PCA9685 library starts with 1
{
	CHIP = chip;
	CHANNEL = channel;
	AJUSTEDMIN = DEFAULTMINSERVOPWM;
	AJUSTEDMAX = DEFAULTMAXSERVOPWM;
	MINSERVOPWM = DEFAULTMINSERVOPWM;
	MAXSERVOPWM = DEFAULTMAXSERVOPWM;
	DIRECTION = 0;
}

MOTOR::MOTOR(PCA9685 *chip, uint8_t channel, uint8_t aMin, uint8_t aMax) // Channel does not start at 0 becuase PCA9685 library starts with 1
{
	CHIP = chip;
	CHANNEL = channel;
	AJUSTEDMIN = DEFAULTMINSERVOPWM + aMin;
	AJUSTEDMAX = DEFAULTMAXSERVOPWM - aMax;
	MINSERVOPWM = DEFAULTMINSERVOPWM;
	MAXSERVOPWM = DEFAULTMAXSERVOPWM;
	DIRECTION = 0;
}

MOTOR::MOTOR(PCA9685 *chip, uint8_t channel, uint8_t aMin, uint8_t aMax, std::string name) // Channel does not start at 0 becuase PCA9685 library starts with 1
{
	CHIP = chip;
	CHANNEL = channel;
	AJUSTEDMIN = DEFAULTMINSERVOPWM + aMin;
	AJUSTEDMAX = DEFAULTMAXSERVOPWM - aMax;
	MINSERVOPWM = DEFAULTMINSERVOPWM;
	MAXSERVOPWM = DEFAULTMAXSERVOPWM;
	NAME = name;
	DIRECTION = 0;
}

MOTOR::MOTOR(PCA9685 *chip, uint8_t channel, uint16_t minservopwm, uint16_t maxservopwm, uint8_t aMin, uint8_t aMax) // Channel does not start at 0 becuase PCA9685 library starts with 1
{
	CHIP = chip;
	CHANNEL = channel;
	AJUSTEDMIN = DEFAULTMINSERVOPWM + aMin;
	AJUSTEDMAX = DEFAULTMAXSERVOPWM - aMax;
	MINSERVOPWM = minservopwm;
	MAXSERVOPWM = maxservopwm;
	DIRECTION = 0;
}

MOTOR::MOTOR(PCA9685 *chip, uint8_t channel, uint16_t minservopwm, uint16_t maxservopwm, uint8_t aMin, uint8_t aMax, std::string name) // Channel does not start at 0 becuase PCA9685 library starts with 1
{
	CHIP = chip;
	CHANNEL = channel;
	AJUSTEDMIN = DEFAULTMINSERVOPWM + aMin;
	AJUSTEDMAX = DEFAULTMAXSERVOPWM - aMax;
	MINSERVOPWM = minservopwm;
	MAXSERVOPWM = maxservopwm;
	NAME = name;
	DIRECTION = 0;
}

/*   getName()
*       returns returns name of Motor
*/
std::string MOTOR::getName()
{
	return NAME;
}

/*   getChannel()
*       returns Channel number on PCA9685 chip (1 - 16)
*/
uint8_t MOTOR::getChannel()
{
	return CHANNEL;
}

/*   getPCA9685()
*       returns PCA9685 chip
*/
PCA9685* MOTOR::getPCA9685()
{
	return CHIP;
}

/*   getAjustedMIN()
*       returns AJUSTMIN value
*/
uint16_t MOTOR::getAjustedMIN()
{
        return AJUSTEDMIN;
}

/*   getMINSERVOPWM()
*       returns MINSERVOPWM value
*/
uint16_t MOTOR::getMINSERVOPWM()
{
	return MINSERVOPWM;
}

/*   getMAXSERVOPWM()
*       returns MAXSERVOPWM value
*/
uint16_t MOTOR::getMAXSERVOPWM()
{
	return MAXSERVOPWM;
}

/*   getAjustedMAX()
*       returns AJUSTMAX value
*/
uint16_t MOTOR::getAjustedMAX()
{
        return AJUSTEDMAX;
}

/*   getAjustedRange()
*       returns Ajusted Range value
*/
uint16_t MOTOR::getAjustedRange()
{
	return AJUSTEDMAX - AJUSTEDMIN;
}

/*   getRange()
*       returns Range value
*/
uint16_t MOTOR::getRange()
{
	return MAXSERVOPWM - MINSERVOPWM;
}

int8_t MOTOR::getDirection()
{
	return DIRECTION;
}

void MOTOR::setDirection(int8_t d)
{
	DIRECTION = d;
}

/*   setAjustedMIN()
*       Sets AJUSTEDMIN to MINSERVOPWM + 'a' so it is 'a' more than MINSERVOPWM
*/
void MOTOR::setAjustedMIN(uint8_t a)
{
	AJUSTEDMIN = MINSERVOPWM + a;
}

/*   setName(String)
*       Sets name of MOTOR
*/
void MOTOR::setName(std::string name)
{
	NAME = name;
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
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	//CHIP->setPWM(CHANNEL, MAXSERVOPWM);
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	CHIP->setPWM(CHANNEL, MINSERVOPWM);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return;
}

/*   armESCWithList(std::vector<MOTOR>)
*       Arms ESC with self and other selected motors
*       Example:
*          motor1.armESCWithList({motor2, motor3, motor4, ...});  <-- any amount of motors can be added
*/
/*
void MOTOR::calibrateESCWithList(std::vector<MOTOR> motors)
{
	uint16_t size = motors.size();
	
	CHIP->setPWM(getchannel(), 0);
	for( uint16_t index = 0; index < size; index++)
		motors.data()[index].getPCA9685()->setPWM(motors.data()[index].getChannel(), 0);

	std::cout << "Set all ESCs PWM to 0\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	CHIP->setPWM(getchannel(), MAXSERVOPWM);
	for( uint16_t index = 0; index < size; index++)
		motors.data()[index].getPCA9685()->setPWM(motors.data()[index].getChannel(), MAXSERVOPWM);

	std::cout << "Set all ESCs PWM to MAX\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	CHIP->setPWM(getchannel(), MINSERVOPWM);
	for( uint16_t index = 0; index < size; index++)
		motors.data()[index].getPCA9685()->setPWM(motors.data()[index].getChannel(), MINSERVOPWM);

	std::cout << "Set all ESCs PWM to MIN\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout << "Calibration and Arming complete\n";

	return;
}
*/

/*   calibrate()
*       Calabrates ESC
*/
void MOTOR::calibrate()
{
	CHIP->setPWM(CHANNEL, 0);
	std::cout << "Disconnect the battery and press enter";
	std::getchar();
	CHIP->setPWM(CHANNEL, MAXSERVOPWM);
	std::cout << "Connect the Battery. After the falling tone, press enter";
	std::getchar();
	CHIP->setPWM(CHANNEL, MINSERVOPWM);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

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
		std::cout << percent << " is outside of range 0 - 100";
}

/*   setSpeed(uint16_t)
*       Sets motor speed from values between 0 and AJUSTEDMAX - AJUSTEDMIN
*/
void MOTOR::setSpeed(uint16_t speed)
{
	if(speed <= (AJUSTEDMAX - AJUSTEDMIN))
		CHIP->setPWM(getChannel(), AJUSTEDMIN + speed);
	else
		std::cout << speed << " is outside of range 0 - " << (AJUSTEDMAX - AJUSTEDMIN) << "\n";
}

// Static functions

/*   armESC()
*       Arms ESC on initalized channel
*	parameters: (*chip1 takes PCA9685 chip as a pointer, channel takes the channel number)
*/
void MOTOR::armESC(PCA9685 *chip1, uint8_t channel)
{
	chip1->setPWM(channel, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//CHIP->setPWM(CHANNEL, MAXSERVOPWM);
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	chip1->setPWM(channel, DEFAULTMINSERVOPWM);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return;
}

void MOTOR::armESC(PCA9685 *chip1, uint8_t channel, uint16_t minservopwm)
{
	chip1->setPWM(channel, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//CHIP->setPWM(CHANNEL, MAXSERVOPWM);
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	chip1->setPWM(channel, minservopwm);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return;
}

/*   armESCFromChannels(PCA9685 *, uint8_t, uint8_t)
*       Arms ESC on initalized channel
*	parameters: (*chip1 takes PCA9685 chip as a pointer, channel takes the channel number)
*/
void MOTOR::armESCFromChannels(PCA9685 *chip1, uint8_t channelStart, uint8_t channelEnd)
{
	for( uint8_t index = channelStart; index <= channelEnd; index++ )
		chip1->setPWM(index, 0);

	std::cout << "Set " << (channelEnd - channelStart) << " ESCs to 0\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	for( uint8_t index = channelStart; index <= channelEnd; index++ )
		chip1->setPWM(index, DEFAULTMINSERVOPWM);

	std::cout << "Set " << (channelEnd - channelStart) << " ESCs to minimum\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout << "Arming complete\n";

	return;
}

/* armESCWithList(std::vector<MOTOR>)
 * 		arms all ESC in the list
 * 
 * Examples: MOTOR::armESCWithList({Motor1, Motor2, Motor3, ...}); --> works with nay nimber of motors
 *			 MOTOR::armESCWithList( (std::vector<MOTOR>) MotorList );
 */
void MOTOR::armESCWithList(std::vector<MOTOR> motors)
{
	uint16_t size = motors.size();
	
	for( uint16_t index = 0; index < size; index++)
		motors.data()[index].getPCA9685()->setPWM(motors.data()[index].getChannel(), 0);

	std::cout << "Set all ESCs PWM to 0\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	for( uint16_t index = 0; index < size; index++)
		motors.data()[index].getPCA9685()->setPWM(motors.data()[index].getChannel(), motors.data()[index].getMINSERVOPWM());

	std::cout << "Set all ESCs PWM to MIN\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout << "Arming complete\n";

	return;
}

/*   SetSpeedPercent(chip, channel, AjustedMin, AjustedMax, speedPercent)
*       Sets motor speed from values between 0 and 100
*/
void MOTOR::setSpeedPercent(PCA9685 *chip1, uint8_t channel, uint8_t aMin, uint8_t aMax, uint8_t percent)
{
	float inc = ((DEFAULTMAXSERVOPWM - aMax) - (DEFAULTMINSERVOPWM + aMin)) / 100.0;

	if (percent <= 100)
		chip1->setPWM(channel, (DEFAULTMINSERVOPWM + aMin + (int) (percent * inc)));
	else
		std::cout << percent << " is outside of range 0 - 100";
}

void MOTOR::setSpeedPercent(PCA9685 *chip1, uint8_t channel, uint16_t minservopwm, uint16_t maxservopwm, uint8_t aMin, uint8_t aMax, uint8_t percent)
{
	float inc = ((maxservopwm - aMax) - (minservopwm + aMin)) / 100.0;

	if(percent <= 100)
		chip1->setPWM(channel, (minservopwm + aMin + (int) (percent * inc)));
	else
		std::cout << percent << " is outside of range 0 - 100";
}

/*   SetSpeed(chip, channel, AjustedMin, AjustedMax, speedPercent)
*       Sets motor speed from values between 0 and DEFAULTMAXSERVOPWM - aMax
*/
void MOTOR::setSpeed(PCA9685 *chip1, uint8_t channel, uint8_t aMin, uint8_t aMax, uint16_t speed)
{
	if(speed <= ((DEFAULTMAXSERVOPWM - aMax) - (DEFAULTMINSERVOPWM + aMin)))
		chip1->setPWM(channel, (DEFAULTMINSERVOPWM + aMin + speed));
	else
		std::cout << speed << " is outside of range 0 - " << ((DEFAULTMAXSERVOPWM - aMax) - (DEFAULTMINSERVOPWM + aMin)) << "\n";
}

void MOTOR::setSpeed(PCA9685 *chip1, uint8_t channel, uint16_t minservopwm, uint16_t maxservopwm, uint8_t aMin, uint8_t aMax, uint16_t speed)
{
	if(speed <= ((maxservopwm - aMax) - (minservopwm + aMin)))
		chip1->setPWM(channel, (minservopwm + aMin + speed));
	else
		std::cout << speed << " is outside of range 0 - " << ((maxservopwm - aMax) - (minservopwm + aMin)) << "\n";
}

/*   calibrate(MOTOR)
*       Calabrates ESC
*/
void MOTOR::calibrateDebug(PCA9685 *chip1, uint8_t channel)
{
	chip1->setPWM(channel, 0);
	std::cout << "Disconnect the battery and press enter";
	std::getchar();
	chip1->setPWM(channel, DEFAULTMAXSERVOPWM);
	std::cout << "Connect the Battery. After the falling tone, press enter";
	std::getchar();
	chip1->setPWM(channel, DEFAULTMINSERVOPWM);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return;
}

/*   calibrate(MOTOR)
*       Calabrates ESC
*/
void MOTOR::calibrate(PCA9685 *chip1, uint8_t channel)
{
	chip1->setPWM(channel, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	chip1->setPWM(channel, DEFAULTMAXSERVOPWM);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	chip1->setPWM(channel, DEFAULTMINSERVOPWM);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return;
}

void MOTOR::calibrate(PCA9685 *chip1, uint8_t channel, uint16_t minservopwm, uint16_t maxservopwm)
{
	chip1->setPWM(channel, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	chip1->setPWM(channel, maxservopwm);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	chip1->setPWM(channel, minservopwm);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return;
}

void MOTOR::calibrateFromChannels(PCA9685 *chip1, uint8_t channelStart, uint8_t channelEnd)
{
	for( uint8_t index = channelStart; index <= channelEnd; index++ )
		chip1->setPWM(index, 0);

	std::cout << "Set " << (channelEnd - channelStart) << " ESCs to 0\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	for( uint8_t index = channelStart; index <= channelEnd; index++ )
		chip1->setPWM(index, DEFAULTMAXSERVOPWM);

	std::cout << "Set " << (channelEnd - channelStart) << " ESCs to maximum (" << DEFAULTMAXSERVOPWM << ")\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	for( uint8_t index = channelStart; index <= channelEnd; index++ )
		chip1->setPWM(index, DEFAULTMINSERVOPWM);

	std::cout << "Set " << (channelEnd - channelStart) << " ESCs to minimum (" << DEFAULTMINSERVOPWM << ")\n";;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout << "Calibration and Arming complete\n";

	return;
}
/* calibrateESCWithList(std::vector<MOTOR>)
 * 		calibrates all ESC in the list
 * 
 * Examples: MOTOR::calibrateESCWithList({Motor1, Motor2, Motor3, ...}); --> works with nay nimber of motors
 *			 MOTOR::calibrateESCWithList( (std::vector<MOTOR>) MotorList );
 */
void MOTOR::calibrateESCWithList(std::vector<MOTOR> motors)
{
	uint16_t size = motors.size();
	
	for( uint16_t index = 0; index < size; index++)
		motors.data()[index].getPCA9685()->setPWM(motors.data()[index].getChannel(), 0);

	std::cout << "Set all ESCs PWM to 0\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	for( uint16_t index = 0; index < size; index++)
		motors.data()[index].getPCA9685()->setPWM(motors.data()[index].getChannel(), motors.data()[index].getMAXSERVOPWM());

	std::cout << "Set all ESCs PWM to MAX\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	for( uint16_t index = 0; index < size; index++)
		motors.data()[index].getPCA9685()->setPWM(motors.data()[index].getChannel(), motors.data()[index].getMINSERVOPWM());

	std::cout << "Set all ESCs PWM to MIN\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout << "Calibration and Arming complete\n";

	return;
}
