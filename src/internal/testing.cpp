/*
  Author:  Nic Draves
  Version: 0.0.2
*/

#include <stdio.h>

#include "PCA9685.h"
#include "MOTOR.h"

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

	m1.armESCList({m1,m2,m3,m4});
	
        return 0;
}


