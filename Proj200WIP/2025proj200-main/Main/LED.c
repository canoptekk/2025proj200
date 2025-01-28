#include <stm32f4xx.h>
#include "LED.h"

void InitLED(void){
	//sets LED to output mode.
	GPIOB->MODER |= 0x10004001; //sets the pin functions for LEDs
	GPIOB->OTYPER &= ~(1UL << 7); //sets blue LED to push-pull
}

