#include <stm32f4xx.h>
#include "PLL_Config.c"
#include "ADC.h"
#include "DAC.h"
#include "lcd.h"
#include "Timer.h"
#include "Buttons.h"
#include "LED.h"
#include "Buzzer.h"


int RedFlash = 0;
int StartUpLock = 0;
int HoldTimer = 0;									//int for hold lock/release
int HoldRelease = 0;
int ButtonMode = 0;							//int for the buttons a, b, c and d
	
unsigned short ADC_DATA_LDR;
unsigned short ADC_DATA_POT;
unsigned short ADC_DATA_MIC;

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
	InitBuzzer();	//config Buzzer
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
			HoldTimer++;
			TIM3->CR1|=TIM_CR1_CEN;
		} else{
			HoldTimer = 0;
			TIM3->CR1&=~TIM_CR1_CEN;
			TIM3->CNT = 0;
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


//TIMER 2 INTERRUPT SERVICE ROUTINE - Samples ADC/Flashes RedLED
void TIM2_IRQHandler(void)			
{
		TIM2->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
	
	//Read values from ADC
		ADC_DATA_LDR=read_adc_LDR();			//LDR output value stored to ADC_DATA_LDR
		ADC_DATA_POT=read_adc_POT();			//POT output value stored to ADC_DATA_POT
		ADC_DATA_MIC=read_adc_MIC();			//MIC output value stored to ADC_DATA_MIC
	/*		
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
*/		
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
		putLCD('H');
		putLCD('O');
		putLCD('L');
		putLCD('D');
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