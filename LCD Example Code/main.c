#include "lcd.h"
#include <stm32f4xx.h>
#include "lcd.h"
#include "lcd.c"
#include <stdio.h>

void start(void){
	
char hbig[]={0x00,0x00,0x66,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7E,0x7E,0x3C,0x3C,0x18,0x00,0x00};

char hsmall[]={0x00,0x00,0x00,0x00,0x00,0x24,0x7E,0x7E,0x3C,0x3C,0x18,0x00,0x00,0x00,0x00,0x00};



//CGRAM Write command will be
//          RS=0 R/W=0       01 2 bit nibble        address higher 2bit nibble     addresslower
//            0     0        01      00                    00 to 00                   0000 to 000F  

// typical 01 00 00 0011 for 4th in list as we start from zero

char CGBASE[0x100];

unsigned char DBASE_at_0x100;

for(int i=0;i<16;i++){
	cmdLCD(CGBASE+chariteration);//addressing a line of pixels
	putLCD(hbig[i]);//write line of pixels
}
CGBASE = CGBASE+16;
for(int i=0;i<16;i++){
	cmdLCD(CGBASE+chariteration);//addressing a line of pixels
	putLCD(hsmall[i]);//write line of pixels
}

putLCD(0x00);
putLCD(0x01);
}
//may need a "write address" function to set display location

int main(void)
{
	initLCD();
	cmdLCD(LCD_LINE1);
	putLCD('A');
	
	while(1);
}
