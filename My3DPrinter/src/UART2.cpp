#include "LPC176x.h"
#include "UART2.h"
#include "stdint.h"




#define ACTUAL_COUNTER 	(rxData_p -> actualCounter)
#define COUNTER			(rxData_p -> counter)
#define STORED_DATA(n)	(rxData_p -> storedData[n])
#define STORE_RX()		STORED_DATA(COUNTER) = U2RBR; COUNTER++;
#define GET_RX(n)		n = STORED_DATA(ACTUAL_COUNTER); ACTUAL_COUNTER++;


extern "C"
{
	static rxData_t *rxData_p;

	void UART2_IRQHandler(void){
		STORE_RX()
	}

}

Uart_2::Uart_2(){

}
Uart_2::~Uart_2(){

}

Uart_2::Uart_2(rxData_t *pointer){

	rxData_p = pointer;
	ACTUAL_COUNTER = 0;
	COUNTER = 0;



	__set_PCONP(3,0);
	__set_PCONP(4,0);
	__set_PCONP(23,0);
	__set_PCONP(24,1);

	__set_PCLKSEL(PCLK_UART2,PCLKDIV_1);

	PINSEL0 &= (~0x00F0000);
	PINSEL4 = ((PINSEL4&(~0x000F0000))|0x000A0000);
	//PINMODE4 = ((PINSEL4&(~0x000F0000))|0x000A0000);




	//BaudRate 115 200 -> 115 384
	U2LCR = 0x80;
	U2DLL = 13;
	U2DLM = 0;

	//8bit/1stop/No parity
	U2LCR = 0x03;

	U2FDR = 0x10;

	//No FIFO
	U2FCR = 0x6;
	//Active Transmit
	U2TER |= 0x80;

	U2IER = 0x01;

	ISER = (ISER&(~(1<<7)))|(1<<7);
	__set_irqn_priority(UART2_IRQn, 0x1F);
}

void Uart_2::sendByte(char data){
	while(!(U2LSR&0x20)){}
	U2THR = data;
}

void Uart_2::sendString(const char *pointer){
	int i = 0;
	while(*(pointer+i) != 0){
		sendByte(*(pointer+i));
		i++;
	}
}

void Uart_2::sendTbl(char *pointer, uint32_t lenght){


	for(uint32_t i = 0; i<lenght; i++){
		sendByte(*(pointer+i));
	}

}

char Uart_2::GetRx(){
	char get;
	GET_RX(get)
	return get;
}

bool Uart_2::dataReady(){
	if(ACTUAL_COUNTER != COUNTER){
		return true;
	}
	else
		return false;
}




