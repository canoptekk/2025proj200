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

int RedFlash = 0;		//holds current state of RedLED - (high or low)
int StartUpLock = 0;	//holds current state of start up loop (locked/unlocked)
int HoldTimer = 0;									//int for hold lock/release
int HoldRelease = 0;						
int ButtonMode = 0;							//int for the buttons a, b, c and d
int BuzzerDuration = 800;
int BuzzerARRValue = 0;
int BuzzerPSCValue = 0;
int LCDTopRow = 0;
int count= 0;

unsigned int i;

int main(void)
{
	//setups and configs
	//PLL_Config();	
	SystemCoreClockUpdate();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOGEN;
	init_ADC();		//config ADC
	init_DAC();		//config DAC
	initLCD();		//config LCD
	InitLED();		//config LED
	InitBut();		//config Buttons
	init_USART(); //config usart
	INIT_BUZZ(BUZZ_PIN); //config Buzzer
	Init_BuzzerPitch();	//configs Delay for buzzer
	//End of setups and configs
	
	
		// Blue Start up button
	while(1)	//forever while - allows program to repeat itself indefinite
	{
		StartUpLock = 0;	//resets StartUpLock value
		while(GPIOC->IDR	& (1<<13)){				//activates when blue button pressed
			
			if(count<=10){
				count++;
				if(count==10){
					StartUpLock++;
						count=0;
				}
		//When blue button is pushed for start, this provides a debouce
		StartUpLock++;	//increases value of int StartUpLock
			
		}                                             //while button is being pressed x will increase
	}
		
		//this provides a small animation like a heart beat when the system starts
		for(int x=0xC0; x<=0xCF; x++){
		 cmdLCD(x);
		putLCD(0xFF);																	//prints block tiles on the bottom row of the LCD left to right
	}
	
	for(int x=0xCF; x>=0xC0; x--){
		 cmdLCD(x);
		putLCD(' ');																	//prints blank tiles from right to left
	}
	
		
		if(0<StartUpLock&StartUpLock<1000000){        //check if x is greater than 0 but less than 1 - run the code 
			
		Init_Sampling();	// Initialise Timer
		Init_HoldButton();	// Initialise Timer	
		//Main loop			
		while(1)	//constantly runs program inside - restarts once complete
		{	
		if(LCDTopRow == 0){
			LCDTopRow = 1;
			
			//LCD display
			cmdLCD(LCD_LINE1);							//displays the heart rate and oxygen on the LCD (1 - 16)
			putLCD('H');	//(1-
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
			putLCD('%');	//-16)
		}
		
		GPIOB->BSRR |= 0x0080; //turns on blue LED
			
		//Hold mode button input
		if(GPIOC->IDR	& (1<<13)){	//activates when blue button pressed
			TIM3->CR1|=TIM_CR1_CEN;	//starts timer3
		} else{
			TIM3->CR1&=~TIM_CR1_CEN;//stops timer3
			TIM3->CNT = 0;	//resets timer3's value back to 0
		}
		
		
		if(	HoldRelease == 0){		//executes enveloped if not in hold mode
			
		
				//4 buttons
		if(GPIOG->IDR & (1<<0)){
			ButtonMode = 0;
				if(count<=10){
				count++;
				if(count==10){
			//When button A is pushed for start, this provides a debouce		
			
			//commands used to modify and update the ARR and PSC to change buzzer tone
			BuzzerARRValue = 1;
			BuzzerPSCValue = 0;
			Init_BuzzerPitch();
			
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
						count=0;
			//When button A is pressed, the bottom row on the LCD will display HEARTRATE and the BPM
			//it will also play one buzz to show it the first option
		/*	
			//buzzer - legacy code kept atm if needed :P
			for(int i=0;i<500;i++){
				GPIOB->ODR ^=(1<<13);
				for(int q=0;q<500;q++){
					__NOP();
				}
			}
		*/
				}
		}
		
	}
		
		
		}
		
	
		
		if(GPIOG->IDR & (1<<1)){
			ButtonMode = 1;	
				if(count<=10){
				count++;
				if(count==10){
					//When button B is pushed for start, this provides a debouce
				}
				cmdLCD(LCD_LINE2);
				putLCD('O');
				putLCD('X');
				putLCD('Y');
				putLCD('G');
				putLCD('E');
				putLCD('N');
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
						count=0;
			//When button B is pressed, the bottom row on the LCD will display OXYGEN and the O2 percentage
			//it will also play two buzzes to show it the second option
				}
		}
		
	
		
		
		if(GPIOG->IDR & (1<<2)){
			ButtonMode = 2;	
				if(count<=10){
				count++;
					if(count==10){
					//When button C is pushed for start, this provides a debouce
				cmdLCD(LCD_LINE2);
				putLCD('H');
				putLCD('U');
				putLCD('M');
				putLCD('I');
				putLCD('D');
				putLCD('I');
				putLCD('T');
				putLCD('Y');
				putLCD('=');
				putLCD('8');
				putLCD('9');
				putLCD('%');
				putLCD(' ');
				putLCD(' ');
				putLCD(' ');
				putLCD(' ');
						count=0;
			//When button C is pressed, the bottom row on the LCD will display HUMIDITY and the percentage
			//it will also play 3 buzzes to show it the third option
				}
		}
		
	}
		

		
		if(GPIOG->IDR & (1<<3)){
			ButtonMode = 3;	
					if(count<=10){
				count++;
				if(count==10){
						//When button D is pushed for start, this provides a debouce
				cmdLCD(LCD_LINE2);
				putLCD('M');
				putLCD('O');
				putLCD('V');
				putLCD('E');
				putLCD('M');
				putLCD('E');
				putLCD('N');
				putLCD('T');
				putLCD('=');
				putLCD('2');
				putLCD('7');
				putLCD('.');
				putLCD('6');
				putLCD('m');
				putLCD('m');
				putLCD(' ');
			//When button D is pressed, the bottom row on the LCD will display MOVEMENT and the distance in milimeters
			//it will also play four buzzes to show it the fourth option
					count=0;
				}
		}
	}
		
}

}
	
}
	
}

//output_dac(ADC_DATA_LDR);			//send straight to DAC (DAC pin should replicate ADC pin)


//TIMER 2 INTERRUPT SERVICE ROUTINE - Samples ADC/Flashes RedLED
void TIM2_IRQHandler(void){
	
		TIM2->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
		
	//Read values from ADC
		POTPuTTyDisplay();
		LDRPuTTyDisplay();
		MICPuTTyDisplay();
	
		//Red flashing circuit
		if(RedFlash == 0){
			GPIOB->BSRR |= 0x40000000; //turns RedLED off
			RedFlash = 1;	//Holds curret state of RedLED
				//turn on buzzer 
		TIM4->CR1|=TIM_CR1_CEN;
		} else{
			GPIOB->BSRR |= 0x4000;	//turns RedLED on
			RedFlash = 0;	//Holds curret state of RedLED
		}
}



//TIMER 3 Hold Mode SERVICE ROUTINE
void TIM3_IRQHandler(void){
	
	if (HoldRelease == 0){
		HoldRelease = 1;
		TIM2->CR1 &= ~TIM_CR1_CEN;	//disables timer 2 (prevents samples being taken)	
		GPIOB->BSRR |= 0x40000000;	//turns off red LED
		cmdLCD(LCD_LINE2);	//moves curser to row 2
		putLCD(' ');	//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    putLCD('H');//writes 'hold' msg onto bottom level of the LCD
    putLCD('O');
    putLCD('L');
    putLCD('D');
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
		
	}else{
		HoldRelease = 0;	//sets hold mode to release
		LCDTopRow = 0;
		TIM2->CR1 |= TIM_CR1_CEN; //resumes timer (sampling)
		cmdLCD(0x01);	//wipes the LCD of characters
		}
	TIM3->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
	TIM3->CR1 &= ~TIM_CR1_CEN;	//disables timer 3
	while(GPIOC->IDR	& (1<<13)){	//prevents system from reading the input to lock as part of the input to unlock
		}
}


//Buzzer timer
void TIM4_IRQHandler(void){	//Duration of buzzer
	
	TIM4->SR&=~TIM_SR_UIF;	//clear interrupt flag in status register
	
	if(i<BuzzerDuration){
		GPIOB->ODR ^=(1<<13);	//turn on buzzer
		i++;
		
	}else{
		TIM4->CR1 &= ~TIM_CR1_CEN;	//turns off timer 4 - (Buzzer Duration)
		TIM4->CNT = 0;	//resets timer4's value back to 0
		i = 0;
	}
	
}




/*commands used for Buzzer
//turn on buzzer 
TIM4->CR1|=TIM_CR1_CEN;

//turn off buzzer
TIM4->CR1 &= ~TIM_CR1_CEN;

//commands used to modify and update the ARR and PSC to change buzzer tone
			BuzzerARRValue = 1;
			BuzzerPSCValue = 0;
			Init_BuzzerPitch();
*/