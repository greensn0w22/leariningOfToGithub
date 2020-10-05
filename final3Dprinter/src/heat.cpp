/*
 * heat.c
 *
 *  Created on: 25 févr. 2020
 *      Author: Julien
 */



/**************************************
 * 		PRIVATE TYPEDEF
 *************************************/

/**************************************
 *			INCLUDES
 *************************************/

#include "LPC176x.h"
#include "heat.h"
#include "math.h"
#include "PID.h"

/**************************************
 *		PRIVATE DEFENITION
 *************************************/
#define PWMMR0		6000
#define MAX_PMW		2300
#define PWM_LATCH()	(PWM1LER = 0x7f)

#define SAMPLE_TIME	20 //mS


#define KP_BLOCK	80	/* Block Proportional Constant (80)*/
#define KI_BLOCK	550	/* Block Integral Constant (500)*/
#define KD_BLOCK	150	/* Block derivative Constant (200)*/


#define KP_BED		10	/* Bed Proportional Constant */
#define KI_BED		40	/* Bed Integral Constant */
#define KD_BED		20	/* Bed derivative Constant */

/**************************************
 *		PRIVATE DECLARATION
 *************************************/

/**************************************
 *		PRIVATE STRUCTURE
 *************************************/

/**************************************
 *		PUBLIC	FUNCTIONS
 *************************************/
heat_c::heat_c(){

	/* Program PWM */
	__set_PCONP(PCPWM1,1);
	__set_PCLKSEL(PCLK_PWM1,PCLKDIV_8);
	__set_PINSEL(1,26,0x02);
	__set_PINOD(1,26,0);

	PWM1TCR = 0x02;	/* Enable counter, PWM and reset ON*/
	PWM1CTCR = 0x0; /* Keep this shit 0 */
	PWM1MCR = 0x02; /* Select reset on PMWMR0*/
	PWM1PCR = 0x6000; /* Enable PWM output*/
	PWM1MR0 = PWMMR0;
	PWM1MR6 = (5999);
	PWM_LATCH();
	PWM1TCR = 0x09;


	/* Set heat pins to be outputs*/
	__set_PINSEL(1,25,0x00);

	__set_PINOD(1,25,0);

	FIO1DIRH |= 0x0600; /* PowerOff at HighLvl */
	FIO1PINH |= 0x0600; /* PowerOff at HighLvl */

	/* Program AD converter */
	__set_PINSEL(1,30,0x03);
	__set_PINSEL(1,31,0x03);
	__set_PINMODE(1,30,2);
	__set_PINMODE(1,31,2);

	__set_PCONP(12,1);
	__set_PCLKSEL(PCLK_ADC,PCLKDIV_8);
	AD0CR = 0x00210030; //12MHz echantillonage

	pidBlock = new PID_c(KP_BLOCK,KI_BLOCK,KD_BLOCK,SAMPLE_TIME,MAX_PMW);






}

heat_c::~heat_c(){
	delete pidBlock;
}

void heat_c::newTemp(heatSource_e source, uint8_t objective){
	if(source == block) pidBlock->setObjective(objective);

}

void heat_c::computePID(heatSource_e source, uint8_t input){
	int newPWM = 1;


	if(source == block){
		newPWM = pidBlock->compute(input);
		PWM1MR6 = PWMMR0-newPWM;
	}
	else if( source == bed){
		//newPWM = pidBed->compute(input);
		PWM1MR5 = PWMMR0 - newPWM;
	}

	PWM_LATCH();
}

uint8_t heat_c::getHeat(heatSource_e source){
	float temperature;
	double parameter;
	uint32_t value;

	if(source == block) value = ((AD0DR5>>4)&0xfff);


	parameter = log((value));

	temperature = (0-41.92)*parameter+365.85;

	return (uint8_t) temperature;
}

/**************************************
 *		PRIVATE	FUNCTIONS
 *************************************/
