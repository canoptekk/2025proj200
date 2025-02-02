#include "usart.h"
#include "ADC.h"
int LDRValue = 0;// value used to hold position of how far in the array of ascii characters have passed
int POTValue = 0;
int MICValue = 0;
int LDRVoltage = 0;
int POTVoltage = 0;
int MICVoltage = 0;
extern int ButtonMode;


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
	 int number = read_adc_LDR();      // The 8-digit number
		LDRVoltage = number*80567;
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
		
		
		
		//voltage output
		char asciiVoltStr[10];           // Array to hold the ASCII string (8 digits + null terminator)
    // Loop to extract each digit from the number and convert to ASCII
    for (int i = 9; i >= 0; i--)  // For an 8-digit number, loop from index 7 to 0
    {
        int digit = LDRVoltage % 10;           // Extract the last digit
        asciiVoltStr[i] = digit + '0';         // Convert digit to ASCII and store in array
        LDRVoltage /= 10;                      // Remove the last digit
    }

    asciiVoltStr[9] = '\0';  // Null-terminate the string
		
		LDRValue=0;
		while(LDRValue<10)//writes full value
		{
		if(LDRValue == 2){
			send_usart('.');
		}
		send_usart(asciiVoltStr[LDRValue]);//writes current ascii character from asciiStr into the terminal
		LDRValue++;	//increases the value of LDRValue which changes the current character in asciiStr to the next in the list
			__NOP();
		}
		send_usart('V');
		send_usart(' ');
		
		
		
	}

	
	
	
	
	
	
	void POTPuTTyDisplay(void){	//writes the value of the LDR onto the command terminal by converting them into ascii characters
	 int numberB = read_adc_POT();      // The 8-digit number
		POTVoltage = numberB*80567;
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
		
		
		//voltage output
		char asciiVoltStr[10];           // Array to hold the ASCII string (8 digits + null terminator)
    // Loop to extract each digit from the number and convert to ASCII
    for (int i = 9; i >= 0; i--)  // For an 8-digit number, loop from index 7 to 0
    {
        int digit = POTVoltage % 10;           // Extract the last digit
        asciiVoltStr[i] = digit + '0';         // Convert digit to ASCII and store in array
        POTVoltage /= 10;                      // Remove the last digit
    }

    asciiVoltStr[9] = '\0';  // Null-terminate the string
		
		POTValue=0;
		while(POTValue<10)//writes full value
		{
		if(POTValue == 2){
			send_usart('.');
		}
		send_usart(asciiVoltStr[POTValue]);//writes current ascii character from asciiStr into the terminal
		POTValue++;	//increases the value of LDRValue which changes the current character in asciiStr to the next in the list
			__NOP();
		}
		send_usart('V');
		send_usart(' ');
		
	}
	
	
	
	
	
	
	
	
	
	
	void MICPuTTyDisplay(void){	//writes the value of the LDR onto the command terminal by converting them into ascii characters
	 int numberC = read_adc_MIC();      // The 8-digit number
		POTVoltage = numberC*80567;
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
		
		//voltage output
		char asciiVoltStr[10];           // Array to hold the ASCII string (8 digits + null terminator)
    // Loop to extract each digit from the number and convert to ASCII
    for (int i = 9; i >= 0; i--)  // For an 8-digit number, loop from index 7 to 0
    {
        int digit = MICVoltage % 10;           // Extract the last digit
        asciiVoltStr[i] = digit + '0';         // Convert digit to ASCII and store in array
        MICVoltage /= 10;                      // Remove the last digit
    }

    asciiVoltStr[9] = '\0';  // Null-terminate the string
		
		MICValue=0;
		while(MICValue<10)//writes full value
		{
		if(MICValue == 2){
			send_usart('.');
		}
		send_usart(asciiVoltStr[MICValue]);//writes current ascii character from asciiStr into the terminal
		MICValue++;	//increases the value of LDRValue which changes the current character in asciiStr to the next in the list
			__NOP();
		}
		send_usart('V');
		send_usart('\r');
		send_usart('\n');
	}
	
	
	void CurrentMode(void){
	
	if(ButtonMode == 0){
		send_usart('H');
		send_usart('E');
		send_usart('A');
		send_usart('R');
		send_usart('T');
		send_usart('R');
		send_usart('A');
		send_usart('T');
		send_usart('E');
		send_usart(' ');
		send_usart(' ');
	}else if(ButtonMode == 1){
		send_usart('O');
		send_usart('X');
		send_usart('Y');
		send_usart('G');
		send_usart('E');
		send_usart('N');
		send_usart(' ');
		send_usart(' ');
	}else if(ButtonMode == 2){
		send_usart('H');
		send_usart('U');
		send_usart('M');
		send_usart('I');
		send_usart('D');
		send_usart('I');
		send_usart('T');
		send_usart('Y');
		send_usart(' ');
		send_usart(' ');
	}else if(ButtonMode == 3){
		send_usart('M');
		send_usart('O');
		send_usart('V');
		send_usart('E');
		send_usart('M');
		send_usart('E');
		send_usart('N');
		send_usart('T');
		send_usart(' ');
		send_usart(' ');
	}
	}