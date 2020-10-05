/*
===============================================================================
 Name        : DoublePWN_C.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include "board.h"
#include "LPCfunctions.h"
#include <cr_section_macros.h>
#include "UART.h"

extern char TBLPrint[8];
extern struct FlagsCMD CMD;

int main(void) {


SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, true);


    EnableUart2(1);
    InitFlagsConst();
    ClrScreen();
    UpdateAllBoard();

    CMD.ActiveDot = 8; //désactive le .

    volatile static int i = 0 ;
    //volatile


    while(1) {

    	if(CMD.FlagInterrupt)
    		CommandLine();
    	i++ ;

    	//U2THR = '1';
    }
    return 0 ;
}
