#include "ADC.h"



void init_ADC(void)
{
	//Initiases the pins that need to be read for ADC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN; //enables all GPIO clocks A, B, and C
	ADC_input_port->MODER|=(3u<<(2*ADC_input_pin));	//ADC input pin is analogue mode (LDR)
	ADC_input_port_POT->MODER|=(3u<<(2*ADC_input_pin_POT));	//ADC input pin is analogue mode (POT)
	ADC_input_port_MIC->MODER|=(3u<<(2*ADC_input_pin_MIC));	//ADC input pin is analogue mode (MIC)
	
	//Initilisation for the LDR
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;		//ADC clock enable
	ADC1->SQR1&=~ADC_SQR1_L;						//set number of conversions per sequence to 1
	ADC1->SQR3&=~ADC_SQR3_SQ1;					//clear channel select bits
	ADC1->SQR3|=ADC_Channel;						//set channel
	ADC1->CR2|=ADC_CR2_ADON;						//enable ADC
	
	//Initilisation for the POT
	RCC->APB2ENR|=RCC_APB2ENR_ADC2EN;		//ADC clock enable
	ADC2->SQR1&=~ADC_SQR1_L;						//set number of conversions per sequence to 1
	ADC2->SQR3&=~ADC_SQR3_SQ1;					//clear channel select bits
	ADC2->SQR3|=ADC_Channel_POT;				//set channel
	ADC2->CR2|=ADC_CR2_ADON;						//enable ADC
	
	//Initilisation for the MIC
	RCC->APB2ENR|=RCC_APB2ENR_ADC3EN;		//ADC clock enable
	ADC3->SQR1&=~ADC_SQR1_L;						//set number of conversions per sequence to 1
	ADC3->SQR3&=~ADC_SQR3_SQ1;					//clear channel select bits
	ADC3->SQR3|=ADC_Channel_MIC;				//set channel
	ADC3->CR2|=ADC_CR2_ADON;						//enable ADC
}


unsigned short read_adc_LDR(void)
{
	ADC1->CR2|=ADC_CR2_SWSTART;				//start ADC conversion
	while((ADC1->SR&ADC_SR_EOC)==0){__NOP();}	//wait for ADC conversion complete
	return ADC1->DR;									//return converted value
}

unsigned short read_adc_POT(void)
{
	ADC2->CR2|=ADC_CR2_SWSTART;				//start ADC conversion
	while((ADC2->SR&ADC_SR_EOC)==0){__NOP();}	//wait for ADC conversion complete
	return ADC2->DR;									//return converted value
}

unsigned short read_adc_MIC(void)
{
	ADC3->CR2|=ADC_CR2_SWSTART;				//start ADC conversion
	while((ADC3->SR&ADC_SR_EOC)==0){__NOP();}	//wait for ADC conversion complete
	return ADC3->DR;									//return converted value
}