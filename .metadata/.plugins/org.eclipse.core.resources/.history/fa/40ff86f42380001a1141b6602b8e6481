#include "LPC176x.h"
#include "printer3dclass.h"



extern flag_t flag;
extern gcodeInfo_t Ginf;
extern gCodeList_t gCodeList;
/*********************
 * INTERRUPT ZONE
 ********************/
#ifdef __cplusplus
extern "C" {

void RIT_IRQHandler(void){
	RICTRL |= 1;
	static uint8_t counter;

	if((++counter)%8){
		switch (counter){
			case 1:
				flag.flagSerialBlockTemp = 1;
				break;
			case 2:
				flag.flagSerialBlockTemp = 1;
				break;
			case 3:
				flag.flagSerialBedTemp = 1;
				break;
			case 4:
				flag.flagSerialBlockTemp = 1;
				break;
			case 5:
				flag.flagSerialBlockTemp = 1;
				break;
			case 6:
				flag.flagSerialBlockTemp = 1;
				break;
			case 7:
				flag.flagSerialProgress = 1;
				break;
		}
	}
	else
		counter = 0;

}

void SysTick_Handler(void){
	SYST_CSR;
	static bool flipflop = 0;
	flag.tenMillisecondSinceOn++;
	if(flag.endLoopFlag)
	{
		if(flipflop)
		{
			flag.flagPIDBlock = 1;
			flag.flagGetHeatBlock = 1;
			flipflop = 0;
		}
		else
		{
			flag.flagPIDBed = 1;
			flag.flagGetHeatBed = 1;
			flipflop = 1;
		}
	}
}

void EINT3_IRQHandler(void){


	while(IO2IntStatR) {

		if((IO2IntStatR&0x0400)){
				Ginf.X = 0;
				Ginf.homeWait = 1;//gCodeList.G0 = 0;
			IO2IntClr = 1<<10;
		}

		else if((IO2IntStatR&0x0800)){
				Ginf.Y = 0;
				Ginf.homeWait = 1;//gCodeList.G0 = 0;
			IO2IntClr = 1<<11;
		}

		else if((IO2IntStatR&0x1000)){
				Ginf.Z = 0;
				Ginf.homeWait = 1;//gCodeList.G0 = 0;
			IO2IntClr = 1<<12;
		}
	}

	IO2IntClr = 0xFFFFFFFF;
	IO0IntClr = 0xFFFFFFFF;

	for (int i=0; i<10000; i++);
}

}
#endif
