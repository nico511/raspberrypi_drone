/*
  Author:  Nic Draves
  Version: 0.0.2
*/

#include <chrono>
#include <thread>
#include <stdio.h>

#include "PCA9685.h"

void wait_ms(int millisec);

int main(void)
{
        printf("Running...\n");
        PCA9685 led1 = PCA9685(1, 0x40);

        for( int index  = 0; index <= 4095; index += 35)
        {
                led1.setPWM(1, index);
                printf("Debug -- index: %d\n", index);
                wait_ms(50);
        }

        wait_ms(3000);

        for( int index  = 4095; index >= 0; index -= 35)
        {
                led1.setPWM(1, index);
                printf("Debug -- index: %d\n", index);
                wait_ms(50);
        }

        return 0;
}

void wait_ms(int millisec)
{
        std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
}
