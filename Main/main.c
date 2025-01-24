#include <stm32f4xx.h>
#include "PLL_Config.c"
#include "ADC.h"
#include "DAC.h"
#include "lcd.h"

int sampling = 0;
int RedFlash = 0;
int StartUpLock = 0;
int HoldTimer = 0;									//int for hold lock/release
int HoldRelease = 0;
int ButtonMode = 0;							//int for the buttons a, b, c and d
	
unsigned short ADC_DATA_LDR;
unsigned short ADC_DATA_POT;
unsigned short ADC_DATA_MIC;
volatile unsigned int i;		//create variable i

int main(void)
{
	PLL_Config();
	SystemCoreClockUpdate();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOGEN;
	init_ADC();		//config ADC
	init_DAC();		//config DAC
	initLCD();		//config LCD
	
	//sets LED to output mode.
	GPIOB->MODER |= 0x10004001; //sets the pin functions for LEDs
	
	//set up buttons A, B, C and D. - Note buttons A and B do not need modifying as registers are set to alt as default
	GPIOG->PUPDR |= 0x20;		//pull up resistor for the GPIOG button C
	GPIOG->PUPDR |= 0x80;		//pull up resistor for the GPIOG button D
	
		// Blue Start up button
	while(1)
	{
		StartUpLock = 0;	//resets StartUpLock value
		
		while(GPIOC->IDR	& (1<<13)){
		StartUpLock++;
		}                                             //while button is being pressed x will increase
		
		if(0<StartUpLock&StartUpLock<1000000){        //check if x is greater than 0 but less than 1 - run the code 
			StartUpLock=0;
			
		//Main loop			
		while(1)
		{	
		
		//Hold mode
		if(GPIOC->IDR	& (1<<13)){
			HoldTimer++;
		} else{
			HoldTimer = 0;
		}
		
		if(HoldTimer<=200000){
			
		//ADC Sampling
		if(sampling <= 100000){
			sampling++;
		}
		else{
		//RESET sampling value
		sampling = 0;
			
		//Read values from ADC
		ADC_DATA_LDR=read_adc_LDR();			//LDR output value stored to ADC_DATA_LDR
		ADC_DATA_POT=read_adc_POT();			//POT output value stored to ADC_DATA_POT
		ADC_DATA_MIC=read_adc_MIC();			//MIC output value stored to ADC_DATA_MIC
			
		//LCD display
		cmdLCD(LCD_LINE1);							//displays the heart rate and oxygen on the LCD
		putLCD('H');
		putLCD('R');
		putLCD('=');
		putLCD('1');
		putLCD('0');
		putLCD('5');
		putLCD(' ');
		putLCD('&');
		putLCD(' ');
		putLCD('O');
		putLCD('2');
		putLCD('=');
		putLCD('9');
		putLCD('8');
		putLCD('.');
		putLCD('7');
		putLCD('%');
		
		//Red flashing circuit
		if(RedFlash == 0){
			GPIOB->BSRR |= 0x40000000;
			RedFlash = 1;
		} else{
			GPIOB->BSRR |= 0x4000;
			RedFlash = 0;
		}
													
		}
	} else{
		//hold mode active
		HoldRelease = 1;
		HoldTimer = 0;
		GPIOB->BSRR |= 0x40000000;	//turns off red LED
		cmdLCD(LCD_LINE2);
		putLCD('H');
		putLCD('O');
		putLCD('L');
		putLCD('D');
		while(GPIOC->IDR	& (1<<13)){
		}
		while(HoldRelease == 1){		//locks system until unlocked with long press
		//Unlock function
		if(GPIOC->IDR	& (1<<13)){
			HoldTimer++;
		} else{
			HoldTimer = 0;
		}
		if(HoldTimer>=200000){
			HoldRelease = 0;
			while(GPIOC->IDR	& (1<<13)){
		}
			cmdLCD(0x01);
		}
		}
	}
	
	//4 buttons
	if(GPIOG->IDR & (1<<0)){
		ButtonMode = 0;
	}
	if(GPIOG->IDR & (1<<1)){
		ButtonMode = 1;
	}
	if(GPIOG->IDR & (1<<2)){
		ButtonMode = 2;
	}
	if(GPIOG->IDR & (1<<3)){
		ButtonMode = 3;
	}
}
}
}
}
//output_dac(ADC_DATA_LDR);			//send straight to DAC (DAC pin should replicate ADC pin)