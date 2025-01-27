#include "usart.h"
int LDRValue = 0;// value used to hold position of how far in the array of ascii characters have passed
int POTValue = 0;
int MICValue = 0;
extern int ADC_DATA_LDR;//output from LDR, taken from main
extern int ADC_DATA_POT;
extern int ADC_DATA_MIC;
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


void LDRPuTTyDisplay(void){	//writes the value of the LDR onto the command terminal by converting them into ascii characters
	 int number = ADC_DATA_LDR;      // The 8-digit number
    char asciiStr[16];           // Array to hold the ASCII string (8 digits + null terminator)
    // Loop to extract each digit from the number and convert to ASCII
    for (int i = 7; i >= 0; i--)  // For an 8-digit number, loop from index 7 to 0
    {
        int digit = number % 10;           // Extract the last digit
        asciiStr[i] = digit + '0';         // Convert digit to ASCII and store in array
        number /= 10;                      // Remove the last digit
    }

    asciiStr[15] = '\0';  // Null-terminate the string

	LDRValue=0;
	while(LDRValue<8)//writes full value
	{
		
		send_usart(asciiStr[LDRValue]);//writes current ascii character from asciiStr into the terminal
		LDRValue++;	//increases the value of LDRValue which changes the current character in asciiStr to the next in the list
			__NOP();
		}
		send_usart(' ');
	}

	void POTPuTTyDisplay(void){	//writes the value of the LDR onto the command terminal by converting them into ascii characters
	 int numberB = ADC_DATA_POT;      // The 8-digit number
    char asciiSBr[16];           // Array to hold the ASCII string (8 digits + null terminator)
    // Loop to extract each digit from the number and convert to ASCII
    for (int i = 7; i >= 0; i--)  // For an 8-digit number, loop from index 7 to 0
    {
        int digit = numberB % 10;           // Extract the last digit
        asciiSBr[i] = digit + '0';         // Convert digit to ASCII and store in array
        numberB /= 10;                      // Remove the last digit
    }

    asciiSBr[15] = '\0';  // Null-terminate the string

	POTValue=0;
	while(POTValue<8)//writes full value
	{
		
		send_usart(asciiSBr[POTValue]);//writes current ascii character from asciiStr into the terminal
		POTValue++;	//increases the value of LDRValue which changes the current character in asciiStr to the next in the list
			__NOP();
		}
		send_usart(' ');
	}
	
	void MICPuTTyDisplay(void){	//writes the value of the LDR onto the command terminal by converting them into ascii characters
	 int numberC = ADC_DATA_MIC;      // The 8-digit number
    char asciiSCr[16];           // Array to hold the ASCII string (8 digits + null terminator)
    // Loop to extract each digit from the number and convert to ASCII
    for (int i = 7; i >= 0; i--)  // For an 8-digit number, loop from index 7 to 0
    {
        int digit = numberC % 10;           // Extract the last digit
        asciiSCr[i] = digit + '0';         // Convert digit to ASCII and store in array
        numberC /= 10;                      // Remove the last digit
    }

    asciiSCr[15] = '\0';  // Null-terminate the string

	MICValue=0;
	while(MICValue<8)//writes full value
	{
		
		send_usart(asciiSCr[MICValue]);//writes current ascii character from asciiStr into the terminal
		MICValue++;	//increases the value of LDRValue which changes the current character in asciiStr to the next in the list
			__NOP();
		}
		send_usart(' ');
	}