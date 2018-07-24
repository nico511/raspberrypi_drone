/*
 *   Title: ESC.h
 *   Description: ESC/servo Library with PCA9685 chip
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

#ifndef GCTRL_H_
#define GCTRL_H_

#include "PCA9685.h"
#include <inttypes.h>

class GCTRL
{
public:
	GCTRL(); //constructor
	void init();
	void init(uint8_t); 
private:
	
};

#endif

