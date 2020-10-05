/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include "board.h"
#include <cr_section_macros.h>
#include "printer3dclass.h"
int main(void) {


    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, true);
    volatile static int i = 0 ;
    printer3d printer;

    while(1) {
    	printer.printerMain();
        i++ ;
    }
    return 0 ;
}
