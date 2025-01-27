#include "Buzzer.h"
#include <stm32f4xx.h>

void 	INIT_BUZZ (char LED_BIT)
{

//enable port (s)
//RCC -> AHB1ENR |= 0X02; //use label defined in stm32f4.h inst. of hex val, easier to read and change
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; //only GPIO B clock enable

// configure port:GPIOB PIN:0 to output for LED1
GPIOB->MODER &= ~(3UL<<(2*LED_BIT)); 			//put in defult states INPUT
GPIOB->MODER |= 1UL<<(2*LED_BIT);					//ONLY SET GPIOB0 , 

GPIOB->OTYPER &= ~ (1UL<<(LED_BIT));	//put in defult states PUSH PULL
//GPIOB->OTYPER |= 1UL<<(LED_BIT); 		//ONLY SET GPIOB0

	GPIOB->OSPEEDR &= ~(3UL<<(2*LED_BIT)); //put in defult states SLOW 2MHZ
	//GPIOB->OSPEEDR |= 2UL<<(2*LED_BIT); //ONLY set Medium
	
		GPIOB->PUPDR &= ~(3UL<<(2*LED_BIT)); //put in defult states no pullup or pull down
	//GPIOB->PUPDR |= 2UL<<(2*LED_BIT); //ONLY set Medium
	
}


void ON (char LED_BIT)
{
	GPIOB-> BSRR = 1<<LED_BIT;  //only turn led ON
}

void OFF (char LED_BIT)
{
	GPIOB-> BSRR = 1<<LED_BIT;  //only turn led OFF
}


//this code was templated after the code from lecture 


	//GPIOB->MODER
	
