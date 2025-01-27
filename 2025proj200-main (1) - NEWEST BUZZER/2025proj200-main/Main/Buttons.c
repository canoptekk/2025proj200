#include "Buttons.h"


void InitBut(void){
	//set up buttons A, B, C and D. - Note buttons A and B do not need modifying as registers are set to alt as default
	GPIOG->PUPDR |= 0x20;		//pull down resistor for the GPIOG button C
	GPIOG->PUPDR |= 0x80;		//pull down resistor for the GPIOG button D
}