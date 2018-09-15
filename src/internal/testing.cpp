/*
  Author:  Nic Draves
  Version: 0.0.2
*/

#include <stdio.h>

#include "PCA9685.h"
#include "MOTOR.h"
#include "functions.h"

// Global chip setup
	PCA9685 chip1 = PCA9685(1, 0x40);
        chip1.setPWMFreq(50);

int main(void)
{
	MOTOR m1 = MOTOR(&chip1, 1);
	//MOTOR m2 = MOTOR(&chip1, 2);
	//MOTOR m3 = MOTOR(&chip1, 3);
	//MOTOR m4 = MOTOR(&chip1, 4);

	//m1.armESCWithList({m2,m3,m4});
	m1.calibrate();
	m1.setAjustedMIN(10);
	m1.setAjustedMAX(10);
	printf("Actual Min: %d - Ajusted Min: %d\n", MINSERVOPWM, m1.getAjustedMIN());
	printf("Actual Max: %d - Ajusted Max: %d\n", MAXSERVOPWM, m1.getAjustedMAX());
	printf("Actual Range: %d - Ajusted Range: %d\n", m1.getRange(), m1.getAjustedRange());

	wait_ms(2000);
	for( uint8_t index = 0; index <= m1.getAjustedRange(); index += 15 ) //30; index++)
	{
		m1.setSpeed(index);
		printf("Debug -- index: %d\n", index);
		wait_ms(500);
	}

	m1.setSpeed(0);

	wait_ms(2000);

	MOTOR::setSpeed(&chip1, 1, 10, 10, 15);

	wait_ms(3000);

	MOTOR::setSpeed(&chip1, 1, 10, 10, 0);

	printf("Done.\n");
        return 0;
}

