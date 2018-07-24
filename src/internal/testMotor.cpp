/*
  Author:  Nic Draves
  Version: 0.0.2
*/

#include <stdio.h>

#include "PCA9685.h"

#define MAXPWM 410
#define MINPWM 145

int main(void)
{
        //Assigns NXP pca9685 to I2C bus 1 (i2c-1) with the address 0x40 (Also inits frequency at 1000hz when function is called)
	PCA9685 pwm = PCA9685(1, 0x40);

	pwm.setPWMFreq(50);
	
	/* calculate pwm value: 
				1/50/4096 = 0.000004882812  0.000700/0.000004882812 = 143.36 
				0.001860/0.000004882812 = 380.928 0.002/0.000004882812 = 409.60
				
				range of pwm: 145 to 410
	*/
	pwm.setPWM(1, 0); // pulse width 0

	printf("Starting motor 1...\n");

	pwm.setPWM(1, 10); //Assigns pwm to slot 0 (written as 1) with a pwm value of 10 (0-4095)

	printf("Debug -- \n");

        wait_ms(1000); //wait 1 second

        return 0;
}
