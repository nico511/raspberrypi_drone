/*
  Author:  Nic Draves
  Version: 0.0.2
*/

#include <stdio.h>

#include "PCA9685.h"
#include "MOTOR.h"
#include "functions.h"

#define MAXPWM 410
#define MINPWM 145

int main(void)
{
        PCA9685 chip1 = PCA9685(1, 0x40);
	chip1.setPWMFreq(50);

	MOTOR m1 = MOTOR(&chip1, 1);
	MOTOR m2 = MOTOR(&chip1, 2);
	MOTOR m3 = MOTOR(&chip1, 3);
	MOTOR m4 = MOTOR(&chip1, 4);

	//m1.armESCWithList({m2,m3,m4});
	m1.calibrate_ARM();

	for( uint8_t index = 0; index <= 100; index++ )
	{
		m1.setSpeedPercent(index);
		printf("Debug -- index: %d\n", index);
		wait_ms(500);
	}

	m1.setSpeedPercent(0);

        return 0;
}


