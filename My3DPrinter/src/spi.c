/*
 * spi.c
 *
 *  Created on: 17 févr. 2020
 *      Author: Julien
 */
/**************************************
 * 		PRIVATE TYPEDEF
 *************************************/
typedef unsigned int	UINT;	/* int must be 16-bit or 32-bit */
typedef unsigned char	BYTE;	/* char must be 8-bit */
typedef	unsigned int  	LBA_t;

/**************************************
 *			INCLUDES
 *************************************/
#include <stdint.h>
#include <stdbool.h>
#include "spi.h"
#include "LPC176x.h"


/**************************************
 *		PRIVATE DEFENITION
 *************************************/
#define BSY 		(SSP1SR & 0x10)
#define CS_LOW() 	FIO0CLR0 = (1 << 6)
#define CS_HIGH() 	FIO0SET0 = (1 << 6)

/**************************************
 *		PRIVATE DECLARATION
 *************************************/
uint16_t spiSpeed = 0;

static uint8_t spi_rw(uint8_t dataByte);
/**************************************
 *		PRIVATE STRUCTURE
 *************************************/


/**************************************
 *		PUBLIC	FUNCTIONS
 *************************************/
bool spi_init(void){
	static uint8_t init = 0;

	if(init == false){
	__set_PCONP(PCSSP1,1);
	__set_PCLKSEL(PCLK_SSP1, PCLKDIV_4); //24KHz

	SSP1DR; //read SSP1FIFO

	__set_PINSEL(0,6,0); //SSEL
	__set_PINSEL(0,7,2); //SCK1
	__set_PINSEL(0,8,2); //MISO
	__set_PINSEL(0,9,2); //MOSI

	//NO pull-up nor pull-down
	__set_PINMODE(0,6,2);
	__set_PINMODE(0,7,2);
	__set_PINMODE(0,8,2);
	__set_PINMODE(0,9,2);

	//
	__set_PINOD(0,6,0);
	__set_PINOD(0,7,0);
	__set_PINOD(0,8,0);

	SSP1CR1 = 2;
	spi_setSPIspeed(_200KHz);

	init = 1;
	}
	return true;
}

uint8_t spi_8bitTransfert(uint8_t data){
	return spi_rw(data);
}

uint16_t spi_16bitTransfert(uint16_t data){
	uint16_t output = 0;
	uint8_t temp[2] = {0};

	temp[1] = (data&0xFF00)>>8;
	temp[0] = (data&0xFF);

	temp[1] = spi_rw(temp[1]);
	temp[0] = spi_rw(temp[0]);

	output  = (temp[1]<<8) + temp[0];

	return output;
}

uint32_t spi_32bitTransfert(uint32_t data){
	uint32_t output = 0;
	uint8_t temp[4] = {0};

	temp[3] = (data&0xFF000000)>>24;
	temp[2] = (data&0x00FF0000)>>16;
	temp[1] = (data&0x0000FF00)>>8;
	temp[0] = (data&0x000000FF);

	temp[3] = spi_rw(temp[3]);
	temp[2] = spi_rw(temp[2]);
	temp[1] = spi_rw(temp[1]);
	temp[0] = spi_rw(temp[0]);

	output  = (temp[3] << 24) + (temp[2] << 16) + (temp[1] << 8) + temp[0];
	return output;
}

void spi_send80DummyClock(){
	//uint8_t dummy;
	FIO0DIR0 = (1 << 6);
	//FIO0SET0 = (1 << 6);
	CS_HIGH();
	__set_PINSEL(0,6,0);
	for(int i = 0; i < 10; i++){
		SSP1DR = 0xFF;
		while(BSY){}
		SSP1DR;

	}
	CS_LOW();
	//FIO0CLR0 = (1 << 6);

}

void spi_send32DummyClock(){
	//uint8_t dummy;
	FIO0DIR0 = (1 << 6);
	FIO0SET0 = (1 << 6);
	__set_PINSEL(0,6,0);
	for(int i = 0; i < 10; i++){
		SSP1DR = 0xFF;
		while(BSY){}
		SSP1DR;

	}
	FIO0CLR0 = (1 << 6);
	__set_PINSEL(0,6,2);

}

void spi_setSPIspeed(spiSpeed_e speed){

	switch (speed) {
		case _200KHz:
			__set_PCLKSEL(PCLK_SSP1, PCLKDIV_4); //24KHz
			SSP1CPSR = 60;
			SSP1CR0 = 0x0107;
			break;
		case _800KHz:
			__set_PCLKSEL(PCLK_SSP1, PCLKDIV_4); //24KHz
			SSP1CPSR = 14;
			SSP1CR0 = 0x0107;
			break;
		case _1MHz:
			__set_PCLKSEL(PCLK_SSP1, PCLKDIV_4); //24KHz
			SSP1CPSR = 12;
			SSP1CR0 = 0x0107;
			break;
		case _2MHz:
			__set_PCLKSEL(PCLK_SSP1, PCLKDIV_4); //24KHz
			SSP1CPSR = 6;
			SSP1CR0 = 0x0107;
			break;
		case _4MHz:
			__set_PCLKSEL(PCLK_SSP1, PCLKDIV_4); //24MHz
			SSP1CPSR = 6;
			SSP1CR0 = 0x0007;
			break;
		case _8MHz:
			__set_PCLKSEL(PCLK_SSP1, PCLKDIV_2); //48KHz
			SSP1CPSR = 6;
			SSP1CR0 = 0x0007;
			break;
		default :
			__set_PCLKSEL(PCLK_SSP1, PCLKDIV_4); //24KHz
			SSP1CPSR = 60;
			SSP1CR0 = 0x0107;
			break;

	}
	spiSpeed = speed;
}

uint16_t spi_giveSPIspeed(void){


	return spiSpeed;
}

bool spi_waitTimer(uint16_t mS){
	uint8_t ans;
	static uint32_t maxLoop;
	static uint16_t lastmS;
	static uint16_t lastSpeed;
	int i;

	if((mS != lastmS) || (lastSpeed != spi_giveSPIspeed()))
	{
		maxLoop = (((spi_giveSPIspeed()*mS)/1000)/8);//((spi_giveSPIspeed()/8)*mS)/((uint16_t) 1000);
		lastmS = mS;
		lastSpeed = spi_giveSPIspeed();
	}

	i=0;
	do{
		ans = spi_8bitTransfert(0xFF);
		i++;
	}while((ans != 0xFF) && (i<maxLoop));

	if(ans == 0xFF) {
		return true;
	}
	return false;
}

bool spi_waitTimerToken(uint16_t mS){
	uint8_t ans;
	static uint32_t maxLoop;
	static uint16_t lastmS;
	static uint16_t lastSpeed;
	int i;

	if((mS != lastmS) || (lastSpeed != spi_giveSPIspeed()))
	{
		maxLoop = (((spi_giveSPIspeed()*mS)/1000)/8);
		lastmS = mS;
		lastSpeed = spi_giveSPIspeed();
	}

	i=0;
	do{
		ans = spi_8bitTransfert(0xFF);
		i++;
	}while((ans == 0xFF) && (i<maxLoop));

	if(ans == 0xFE) {
		return true;
	}
	return false;
}

void spi_receiveMultiByte(
	BYTE *buffer,
	UINT byteToReceive /* 16,64 or 512 byte */
)
{
	uint16_t i;

	//for(i = 0; i<4; i++){
	//	spi_32bitTransfert(0xFFFFFFFF);
	//} //push dummy frames into tx fifo
	i=0;
	while(byteToReceive > 0){
		*(buffer+i) = spi_8bitTransfert(0xFF);
		i++;
		byteToReceive--;
	}
}

void spi_sendMultiByte(
	const BYTE *buffer,
	UINT byteToSend /* in multiple of 16 */
)
{
	uint16_t temp;

	while(byteToSend > 0){
		temp = *buffer++;    			//todo surveiller ça
		temp = ((temp<<8)|*buffer++);
		spi_16bitTransfert(temp);
		byteToSend -= 2;
	}
}

bool spi_select(void){

	CS_LOW();

	spi_8bitTransfert(0xFF);

	if(spi_waitTimer(500)) {return 1;}

	else
		{spi_deselect();}

	return 0;
}

void spi_deselect (void)
{
	CS_HIGH();
	spi_8bitTransfert(0xFF);

}



/*-----------------------------------------------------------------------*/
/* Select card and wait for ready                                        */
/*-----------------------------------------------------------------------*/

/**************************************
 *		PRIVATE FUNCTIONS
 *************************************/
static uint8_t spi_rw(uint8_t dataByte){
	SSP1DR = dataByte;
	while(SSP1SR & 0x10);

	return SSP1DR;
}
