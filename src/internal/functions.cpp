/*
 *   Title: functions.cpp
 *   Description: Functions definitions for the functions.h header
 *
 *   Written by Nic Draves
 *   version: 0.0.1
 */

#include "functions.h"

void wait_ms(int millisec)
{
        std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
	return;
}

