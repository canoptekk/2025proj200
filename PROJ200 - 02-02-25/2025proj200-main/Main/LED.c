#include <stm32f4xx.h>
#include "LED.h"

void InitLED(void){
	//sets LED to output mode.
	
	GPIOB->MODER &= ~((3UL << (2 * 14)) | (3UL << (2 * 7)) | (3UL << (2 * 0)) ); //sets MODER for register 0, 7 and 14 to input/reset state
	GPIOB->MODER |= 0x10004001; //sets the pin functions for LEDs
	
	
	GPIOB->OTYPER &= ~(1UL << 7) | (1UL << 14) | (1UL << 0); //sets LEDs to push-pull
	
	GPIOB->OSPEEDR &= ~((3UL << (2 * 14)) | (3UL << (2 * 7)) | (3UL << (2 * 0)) ); //sets speed of register 0, 14 and 7 to default (slow speed)
	GPIOB->OSPEEDR |= ((2UL << (2 * 14)) | (2UL << (2 * 7)) | (2UL << (2 * 0)) );	//sets speed of register 0, 14 and 7 to medium speed
	
	GPIOB->PUPDR &= ~((3UL << (2 * 14)) | (3UL << (2 * 7)) | (3UL << (2 * 0)) );
}

