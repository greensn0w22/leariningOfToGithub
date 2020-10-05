/*
 * UART2.h
 *
 *  Created on: 7 févr. 2020
 *      Author: Julien
 */

#ifndef UART2_H_
#define UART2_H_

#include <stdint.h>

extern "C"{
	typedef struct {
		uint8_t counter;
		uint8_t actualCounter;
		char storedData[256];
	}rxData_t;
}



class Uart_2 {
public	: //variables

private	: //variables

public 	: //functions

	Uart_2(rxData_t *);
	~Uart_2();

	void sendString(const char *);

	void sendTbl(char *, uint32_t);

	char GetRx(void);

	bool dataReady(void);


private	: //functions
	Uart_2();

	void sendByte(char data);

};


#endif /* UART2_H_ */
