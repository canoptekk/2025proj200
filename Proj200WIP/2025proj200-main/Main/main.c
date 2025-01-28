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

	
unsigned short ADC_DATA_LDR;	//stores digital output of the LDR
unsigned short ADC_DATA_POT;//stores digital output of the POT
unsigned short ADC_DATA_MIC;//stores digital output of the MIC
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

	
	INIT_BUZZ(BUZZ_PIN);
	//End of setups and configs
	
	
		// Blue Start up button
	while(1)	//forever while - allows program to repeat itself indefinite
	{
		StartUpLock = 0;	//resets StartUpLock value
		while(GPIOC->IDR	& (1<<13)){		//activates when blue button pressed
		StartUpLock++;	//increases value of int StartUpLock
		}                                             //while button is being pressed x will increase
		
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
	
	}
		//LCD display
		cmdLCD(LCD_LINE1);							//displays the heart rate and oxygen on the LCD (1 - 16)
		putLCD('H');	//(1-
		putLCD('R');
		putLCD('=');
		putLCD('1');
		putLCD('0');
		putLCD('5');
		putLCD('&');
		putLCD('O');
		putLCD('2');
		putLCD('=');
		putLCD('9');
		putLCD('8');
		putLCD('.');
		putLCD('7');
		putLCD('%');
		putLCD(0xFF);
											//-16)
		
	LDRPuTTyDisplay(); //used to call function - writes LDR value onto command terminal
	//POTPuTTyDisplay();
	//MICPuTTyDisplay();
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
		cmdLCD(LCD_LINE2);	//moves curser to row 2
		putLCD(' ');	//fills space on screen with a blank
    ON(BUZZ_PIN);//plays tune - (activates buzzer)
    putLCD(' ');//fills space on screen with a blank
    OFF(BUZZ_PIN);//plays tune - (deactivates buzzer)
    putLCD(' ');//fills space on screen with a blank
    ON(BUZZ_PIN);//plays tune - (activates buzzer)
    putLCD(' ');//fills space on screen with a blank
    OFF(BUZZ_PIN);//plays tune - (deactivates buzzer)
    putLCD(' ');//fills space on screen with a blank
    ON(BUZZ_PIN);//plays tune - (activates buzzer)
    putLCD(' ');//fills space on screen with a blank
    OFF(BUZZ_PIN);//plays tune - (deactivates buzzer)
    putLCD('H');//writes 'hold' msg onto bottom level of the LCD
    putLCD('O');
    ON(BUZZ_PIN);//plays tune - (activates buzzer)
    putLCD('L');
    putLCD('D');
    OFF(BUZZ_PIN);//plays tune - (deactivates buzzer)
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    ON(BUZZ_PIN);//plays tune - (activates buzzer)
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank
    OFF(BUZZ_PIN);//plays tune - (deactivates buzzer)
    putLCD(' ');//fills space on screen with a blank
    putLCD(' ');//fills space on screen with a blank

	}else{
		HoldRelease = 0;	//sets hold mode to release
		TIM2->CR1 |= TIM_CR1_CEN; //resumes timer (sampling)
		cmdLCD(0x01);	//wipes the LCD of characters
		}
	TIM3->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
	TIM3->CR1 &= ~TIM_CR1_CEN;	//disables timer 3
	while(GPIOC->IDR	& (1<<13)){	//prevents system from reading the input to lock as part of the input to unlock
		}
}





/*#include "lcd.h"
#include "lcd.c"
#include <stdio.h>
void start(void){
	
char hbig[]={0x00,0x00,0x66,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7E,0x7E,0x3C,0x3C,0x18,0x00,0x00};

char hsmall[]={0x00,0x00,0x00,0x00,0x00,0x24,0x7E,0x7E,0x3C,0x3C,0x18,0x00,0x00,0x00,0x00,0x00};


//CGRAM Write command will be
//          RS=0 R/W=0       01 2 bit nibble        address higher 2bit nibble     addresslower
//            0     0        01      00                    00 to 00                   0000 to 000F  

// typical 01 00 00 0011 for 4th in list as we start from zero


CGBASE(0x100);

DBASE 0x1mmm nnnn //where mmm is UPPER part-byte and nnnn is lower byte of Display address

for(int i=0;i<16;i++){
	cmdLCD(CGBASE+chariteration);//addressing a line of pixels
	putLCD(hbig[i]);//write line of pixels
}
CGBASE =CGBASE+16;
for(int i=0;i<16;i++){
	cmdLCD(CGBASE+chariteration);//addressing a line of pixels
	putLCD(hsmall[i]);//write line of pixels
}

putLCD(0x00);
putLCD(0x01);
}
//may need a "write address" function to set display location*/
