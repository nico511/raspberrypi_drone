/*
  Author:  Nic Draves
  Version: 0.0.2
*/

#include <iostream>
#include "functions.h"
#include "GCTRL.h"

int main(void)
{
	GCTRL drone = GCTRL();

	drone.PCA9685Setup(1, 0x40, 50);
	drone.addMotor(0, 1, 10, 10, "UpperLeft");
	drone.addMotor(0, 2, 10, 10, "UpperRight");
	drone.addMotor(0, 3, 10, 10, "BottomLeft");
	drone.addMotor(0, 4, 10, 10, "BottomRight");
	drone.calibrate(); // or drone.arm()


	return 0;
}

