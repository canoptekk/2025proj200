#include "Buttons.h"


void InitBut(void){
	//set up buttons A, B, C and D. - Note buttons A and B do not need modifying as registers are set to alt as default
	
	GPIOC->MODER &= ~((3UL << (2 * 13))); //resets blue button to reset state(input)
	GPIOG->MODER &= ~((3UL << (2 * 0)) | (3UL << (2 * 1)) | (3UL << (2 * 2)) | (3UL << (2 * 3)));	//resets button A, B, C and D to reset state (input)
	
	GPIOC->OTYPER &= ~(1<<13);
	GPIOG->OTYPER &= ~(1<<0) | (1<<1) | (1<<2) | (1<<3);
	
	
	GPIOG->PUPDR |= 0x20;		//pull down resistor for the GPIOG button C
	GPIOG->PUPDR |= 0x80;		//pull down resistor for the GPIOG button D
	GPIOG->PUPDR &= ~(3<<(2*0)) | (3<<(2*1));
	GPIOC->PUPDR &= ~(3<<(2*13));
}