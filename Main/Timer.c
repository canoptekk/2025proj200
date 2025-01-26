#include "Timer.h"
#include "LED.h"

void Init_Sampling(void)
{
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;		//timer 2 clock enabled
	TIM2->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM2->PSC=256-1;										//divide APB clock by 256 = 90MHz/256 = 351kHz
	TIM2->ARR=35156;										//counter reload value, gives a timer period of 100ms when F_APB = 90MHz and PSC = 256
	TIM2->CNT=0;												//zero timer counter
	NVIC->ISER[0]|=(1u<<28);						//timer 2 global interrupt enabled
	TIM2->CR1|=TIM_CR1_CEN;							//start timer counter
}

void Init_HoldButton(void)
{
	RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;		//timer 3 clock enabled
	TIM3->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM3->PSC=256-1;										//divide APB clock by 256 = 90MHz/256 = 351kHz
	TIM3->ARR=60000;										//counter reload value, gives a timer period of 100ms when F_APB = 90MHz and PSC = 256
	TIM3->CNT=0;												//zero timer counter
	NVIC->ISER[0]|=(1u<<29);						//timer 3 global interrupt enabled
	//TIM3->CR1|=TIM_CR1_CEN;							//start timer counter
}
/*
void GreenLED(void)
{
	int ARRValue = GreenModeArray[ButtonMode];
	RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;		//timer 3 clock enabled
	TIM4->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM4->PSC=256-1;										//divide APB clock by 256 = 90MHz/256 = 351kHz
	TIM4->ARR=ARRValue;										//counter reload value, gives a timer period of 100ms when F_APB = 90MHz and PSC = 256
	TIM4->CNT=0;												//zero timer counter
	NVIC->ISER[0]|=(1u<<30);						//timer 3 global interrupt enabled
	TIM4->CR1|=TIM_CR1_CEN;							//start timer counter
}
*/