/*
  Author:  Nic Draves
  Version: 0.0.2

*/
#include <stdio.h>

#include "PCA9685.h"
#include "MOTOR.h"
//#include "functions.h"

int main(int argc, char *argv[])
{
	printf("argc: %d\nargv[0]\n", argc, argv[0]);

	if( argc == 1 )
		printf("no arguments\n");
	else
	{
        	PCA9685 chip1 = PCA9685(1, 0x40);
		for( int index = 1; index < argc; ++index )
		{
        		MOTOR(&chip1, index)::SetSpeed(0);
		}
	}

	return 0;
}
