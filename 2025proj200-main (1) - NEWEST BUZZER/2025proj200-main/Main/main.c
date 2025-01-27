#include <stm32f4xx.h>
#include "PLL_Config.c"
#include "ADC.h"
#include "DAC.h"
#include "lcd.h"
#include "Timer.h"
#include "Buttons.h"
#include "LED.h"
#include "Buzzer.h"
#include <stdio.h>
#include "usart.h"
#include "main.h"

#define BUZZ_PIN 13

int RedFlash = 0;
int StartUpLock = 0;
int HoldTimer = 0;									//int for hold lock/release
int HoldRelease = 0;
int ButtonMode = 0;							//int for the buttons a, b, c and d

	
unsigned short ADC_DATA_LDR;
unsigned short ADC_DATA_POT;
unsigned short ADC_DATA_MIC;
unsigned int i;

int main(void)
{
	//setups and configs
	PLL_Config();
	SystemCoreClockUpdate();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOGEN;
	init_ADC();		//config ADC
	init_DAC();		//config DAC
	initLCD();		//config LCD
	InitLED();		//config LED
	InitBut();		//config Buttons
	init_USART(); //config usart

	
	INIT_BUZZ(BUZZ_PIN);
	//GreenLED();
	//End of setups and configs
	
	
		// Blue Start up button
	while(1)
	{
		StartUpLock = 0;	//resets StartUpLock value
		while(GPIOC->IDR	& (1<<13)){
		StartUpLock++;
		}                                             //while button is being pressed x will increase
		
		if(0<StartUpLock&StartUpLock<1000000){        //check if x is greater than 0 but less than 1 - run the code 
			
		Init_Sampling();	// Initialise Timer
		Init_HoldButton();	// Initialise Timer	
		//Main loop			
		while(1)	//constantly runs program inside - restarts once complete
		{	
		
		GPIOB->BSRR |= 0x0080; //turns on blue LED
			
		//Hold mode button input
		if(GPIOC->IDR	& (1<<13)){
			TIM3->CR1|=TIM_CR1_CEN;
		} else{
			TIM3->CR1&=~TIM_CR1_CEN;
			TIM3->CNT = 0;
		}
		
		
		
		
	if(	HoldRelease == 0){
		
				//4 buttons
		if(GPIOG->IDR & (1<<0)){
			ButtonMode = 0;
				cmdLCD(LCD_LINE2);
				ON(BUZZ_PIN);
				putLCD('H');
				putLCD('E');
				OFF(BUZZ_PIN);
				putLCD('A');
				putLCD('R');
				putLCD('T');
				putLCD('R');
				putLCD('A');
				putLCD('T');
				putLCD('E');
				putLCD('=');
				putLCD('1');
				putLCD('0');
				putLCD('6');
				putLCD('B');
				putLCD('P');
				putLCD('M');
			//When button A is pressed, the bottom row on the LCD will display HEARTRATE and the BPM
			//it will also play one buzz to show it the first option
		}
		if(GPIOG->IDR & (1<<1)){
			ButtonMode = 1;	
				cmdLCD(LCD_LINE2);
				ON(BUZZ_PIN);
				putLCD('O');
				putLCD('X');
				OFF(BUZZ_PIN);
				putLCD('Y');
				putLCD('G');
				ON(BUZZ_PIN);
				putLCD('E');
				putLCD('N');
				OFF(BUZZ_PIN);
				putLCD('=');
				putLCD('9');
				putLCD('8');
				putLCD('.');
				putLCD('7');
				putLCD('%');
				putLCD(' ');
				putLCD(' ');
				putLCD(' ');
				putLCD(' ');
			//When button B is pressed, the bottom row on the LCD will display OXYGEN and the O2 percentage
			//it will also play two buzzes to show it the second option
		}
		if(GPIOG->IDR & (1<<2)){
			ButtonMode = 2;	

				cmdLCD(LCD_LINE2);
				ON(BUZZ_PIN);
				putLCD('H');
				putLCD('U');
				OFF(BUZZ_PIN);
				putLCD('M');
				putLCD('I');
				ON(BUZZ_PIN);
				putLCD('D');
				putLCD('I');
				OFF(BUZZ_PIN);
				putLCD('T');
				putLCD('Y');
				ON(BUZZ_PIN);
				putLCD('=');
				putLCD('8');
				OFF(BUZZ_PIN);
				putLCD('9');
				putLCD('%');
				putLCD(' ');
				putLCD(' ');
				putLCD(' ');
				putLCD(' ');
			//When button C is pressed, the bottom row on the LCD will display HUMIDITY and the percentage
			//it will also play 3 buzzes to show it the third option
		}
		if(GPIOG->IDR & (1<<3)){
			ButtonMode = 3;	
				
				cmdLCD(LCD_LINE2);
				ON(BUZZ_PIN);
				putLCD('M');
				putLCD('O');
				OFF(BUZZ_PIN);
				putLCD('V');
				putLCD('E');
				ON(BUZZ_PIN);
				putLCD('M');
				putLCD('E');
				OFF(BUZZ_PIN);
				putLCD('N');
				putLCD('T');
				ON(BUZZ_PIN);
				putLCD('=');
				putLCD('2');
				OFF(BUZZ_PIN);
				putLCD('7');
				putLCD('.');
				ON(BUZZ_PIN);
				putLCD('6');
				putLCD('m');
				OFF(BUZZ_PIN);
				putLCD('m');
				putLCD(' ');
			//When button D is pressed, the bottom row on the LCD will display MOVEMENT and the distance in milimeters
			//it will also play four buzzes to show it the fourth option
		}
	/*	
		//4 buttons
		if(GPIOG->IDR & (1<<0)){
			ButtonMode = 0;	
			cmdLCD(LCD_LINE2);
			putLCD('H');
		}
		if(GPIOG->IDR & (1<<1)){
			ButtonMode = 1;	
			cmdLCD(LCD_LINE2);
			putLCD('P');
		}
		if(GPIOG->IDR & (1<<2)){
			ButtonMode = 2;	
		}
		if(GPIOG->IDR & (1<<3)){
			ButtonMode = 3;	
		}
		*/
	}
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
		
	LDRPuTTyDisplay();
}
}
}
}
//output_dac(ADC_DATA_LDR);			//send straight to DAC (DAC pin should replicate ADC pin)


//TIMER 2 INTERRUPT SERVICE ROUTINE - Samples ADC/Flashes RedLED
void TIM2_IRQHandler(void)			
{
		TIM2->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
	
	//Read values from ADC
		ADC_DATA_LDR=read_adc_LDR();			//LDR output value stored to ADC_DATA_LDR
		ADC_DATA_POT=read_adc_POT();			//POT output value stored to ADC_DATA_POT
		ADC_DATA_MIC=read_adc_MIC();			//MIC output value stored to ADC_DATA_MIC
	
		//Red flashing circuit
		if(RedFlash == 0){
			GPIOB->BSRR |= 0x40000000; //turns RedLED off
			RedFlash = 1;	//Holds curret state of RedLED
		} else{
			GPIOB->BSRR |= 0x4000;	//turns RedLED on
			RedFlash = 0;	//Holds curret state of RedLED
		}
}



//TIMER 3 Hold Mode SERVICE ROUTINE
void TIM3_IRQHandler(void)			
{
	
	if (HoldRelease == 0){
		HoldRelease = 1;
		TIM2->CR1 &= ~TIM_CR1_CEN;	//disables timer 2 (prevents samples being taken)	
		GPIOB->BSRR |= 0x40000000;	//turns off red LED
		cmdLCD(LCD_LINE2);
		putLCD(' ');
    ON(BUZZ_PIN);
    putLCD(' ');
    OFF(BUZZ_PIN);
    putLCD(' ');
    ON(BUZZ_PIN);
    putLCD(' ');
    OFF(BUZZ_PIN);
    putLCD(' ');
    ON(BUZZ_PIN);
    putLCD(' ');
    OFF(BUZZ_PIN);
    putLCD('H');
    putLCD('O');
    ON(BUZZ_PIN);
    putLCD('L');
    putLCD('D');
    OFF(BUZZ_PIN);
    putLCD(' ');
    putLCD(' ');
    ON(BUZZ_PIN);
    putLCD(' ');
    putLCD(' ');
    OFF(BUZZ_PIN);
    putLCD(' ');
    putLCD(' ');

	}else{
		HoldRelease = 0;
		TIM2->CR1 |= TIM_CR1_CEN; //resumes timer (sampling)
		cmdLCD(0x01);
		}
	TIM3->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
	TIM3->CR1 &= ~TIM_CR1_CEN;	//disables timer 3
	while(GPIOC->IDR	& (1<<13)){	//prevents system from reading the input to lock as part of the input to unlock
		}
}