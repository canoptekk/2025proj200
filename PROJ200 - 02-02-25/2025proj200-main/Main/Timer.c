#include "Timer.h"
#include "LED.h"

extern int BuzzerARRValue;
extern int BuzzerPSCValue;
int ARRArray[] = {100, 300, 500, 800};
int PSCArray[] = {16,};

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

void Init_BuzzerPitch(void)
{
	
	RCC->APB1ENR|=RCC_APB1ENR_TIM5EN;		//timer 4 clock enabled
	TIM5->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM5->PSC=PSCArray[BuzzerPSCValue]-1;										//divide APB clock by 256 = 90MHz/256 = 351kHz
	TIM5->ARR=ARRArray[BuzzerARRValue];										//counter reload value, gives a timer period of 100ms when F_APB = 90MHz and PSC = 256
	TIM5->CNT=0;												//zero timer counter
	NVIC->ISER[1] |= (1u << (50 - 32));		//ISER is 32 bit so location 50 used for tim5 is stored on the second ISER (1)
	//NVIC->ISER[0]|=(1u<<30);						//timer 4 global interrupt enabled
	//TIM4->CR1|=TIM_CR1_CEN;							//start timer counter
}


/*
void GreenLED(void)
{
	int ARRValue = GreenModeArray[ButtonMode];
	RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;		//timer 3 clock enabled
	TIM6->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM6->PSC=256-1;										//divide APB clock by 256 = 90MHz/256 = 351kHz
	TIM6->ARR=ARRValue;										//counter reload value, gives a timer period of 100ms when F_APB = 90MHz and PSC = 256
	TIM6->CNT=0;												//zero timer counter
	NVIC->ISER[0]|=(1u<<30);						//timer 3 global interrupt enabled
	TIM6->CR1|=TIM_CR1_CEN;							//start timer counter
}
*/