#include "usart.h"
int LDRValue = 0;
void send_usart(unsigned char d)
{
	while(!(USART_MODULE->SR & USART_SR_TC));
	USART_MODULE->DR=d;												//write byte to usart data register
}



void init_USART(void)
{
	unsigned char i1,i2;
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;		//usart port clock enable
	
	USART_PORT->MODER&=~(										//clear pin function bits
		(3u<<(2*USART_TX_pin))
		|(3u<<(2*USART_RX_pin))
			);
	USART_PORT->MODER|=(										//reset pin function bits (alternate function)
		(2u<<(2*USART_TX_pin))
		|(2u<<(2*USART_RX_pin))
			);
	
	i1=USART_TX_pin/8;											// used to select AFR[reg] for TX_pin ... pins 0-7 AFR[0] pins 8-15 AFR[1]
	i2=USART_RX_pin>>3;											// same as above for RX_pin (uses shift to divide)

		// ALTERNATE FUNCTION SELECT BITS
	USART_PORT->AFR[i1]&=~(0x0f<<(4*(USART_TX_pin-(i1*8))));    //clear pin function
	USART_PORT->AFR[i1]|=(0x07<<(4*(USART_TX_pin-(i1*8))));			// set usart as alternate function for TX_pin
	USART_PORT->AFR[i2]&=~(0x0f<<(4*(USART_RX_pin-(i2*8))));		// clear pin function
	USART_PORT->AFR[i2]|=(0x07<<(4*(USART_RX_pin-(i2*8))));			//set usart as alternate function for RX_pin
	
	RCC->APB1ENR|=RCC_APB1ENR_USART3EN;			//usart clock enable
	USART_MODULE->CR1|=(										//USART CONFIG
			USART_CR1_TE												//transmit enable
			|USART_CR1_RE												//receive enable
			|USART_CR1_UE												//usart main enable bit
				);
				
	//USART_MODULE->BRR = 0x0683;
				/*
	int CalcBaud = SystemCoreClock / BAUDRATE;  // Integer part
    int fraction = (SystemCoreClock % BAUDRATE) * 16 / BAUDRATE;  // Fractional part
    int BaudRate = (CalcBaud << 4) | fraction;  // Combine integer and fractional parts
    USART_MODULE->BRR = BaudRate;
	*/
	USART_MODULE->BRR=SystemCoreClock/(BAUDRATE);		//set baud rate
																									//this is not an accurate way to set the baudrate
																									//and only works for 16Mhz system clock.
																									
}

void NumberToAscii(int number, char *asciiStr)
{
    for (int i = 7; i >= 0; i--)  // For 8 digits, loop from 7 to 0
    {
        int digit = number % 10;  // Extract the last digit
        asciiStr[i] = digit + '0';  // Convert digit to ASCII and store in string
        number /= 10;  // Remove the last digit from the number
    }

    asciiStr[8] = '\0';  // Null-terminate the string
}

void LDRPuTTyDisplay(void){
	 int number = 12345678;      // The 8-digit number
    char asciiStr[9];           // Array to hold the ASCII string (8 digits + null terminator)
    // Loop to extract each digit from the number and convert to ASCII
    for (int i = 7; i >= 0; i--)  // For an 8-digit number, loop from index 7 to 0
    {
        int digit = number % 10;           // Extract the last digit
        asciiStr[i] = digit + '0';         // Convert digit to ASCII and store in array
        number /= 10;                      // Remove the last digit
    }

    asciiStr[8] = '\0';  // Null-terminate the string

	
	while(LDRValue<8)
	{
		
		send_usart(asciiStr[LDRValue]);
		LDRValue++;
			__NOP();
		}
		
	}