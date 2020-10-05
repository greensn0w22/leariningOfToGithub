/*
===============================================================================
 Name        : SPItest.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include "board.h"
#include <cr_section_macros.h>


static void CoreInit(void){
    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, true);
}

static void flip(void){
	int i = 0;
	for (i = 0; i < 5000; i++){}
	FIO0SET1 |= 0x4;
	for (i = 0; i < 10; i++){}
	FIO0CLR1 |= 0x4;
}

int main(void) {
	CoreInit();
    volatile static int i = 0 ;

    __set_PINMODE(0,10,0); //pwm 0.10
    __set_PINMODE(0,19,0); //dir 0.19
    __set_PINMODE(2, 7,0); //sleep ZZ
    __set_PINMODE(2, 6,0); //sleep EXY

    FIO0DIR  |= 0x00080400; //avtive les bit
    FIO2DIR0 |= 0xC0;

    FIO2CLR0 |= 0xC0;
    FIO0CLR  |= 0x00080400;
    FIO2SET0 |= 0x40; //unsleep EXY


    int j = 0;
    bool test = 0;
    while(1) {

    	for(j = 0; j< (200*32); j++)
    	{
    		flip();
    	}


    	if(test == true)
    	{
    		test = 0;
    	}
    	else{
    		test = 1;
    	}

    	FIO0PIN = (FIO0PIN&(~0x00080000))|(test<<19);

    	for(j=0; j<10000000; j++){}

    	i++;
    }
    return 0 ;
}




