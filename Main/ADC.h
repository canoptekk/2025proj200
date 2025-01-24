#ifndef _ADC_H
#define _ADC_H
#include <stm32f4xx.h>

//for LDR
#define ADC_input_port		GPIOC
#define ADC_input_pin			0
#define ADC_Channel				10 //this changes the port location

//for POT
#define ADC_input_port_POT		GPIOA
#define ADC_input_pin_POT			0
#define ADC_Channel_POT				0

//for MIC
#define ADC_input_port_MIC		GPIOA
#define ADC_input_pin_MIC			0
#define ADC_Channel_MIC				3





void init_ADC(void);
unsigned short read_adc_LDR(void);
unsigned short read_adc_POT(void);
unsigned short read_adc_MIC(void);
#endif 








