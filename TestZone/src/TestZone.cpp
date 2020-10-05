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

#include "heat.h"


int main(void) {

    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, true);


    volatile static int i = 0 ;
    //char tbl[10];

    heat_c heat;

    while(1) {

        i++ ;
    }
    return 0 ;
}
