/*
 * spi.h
 *
 *  Created on: 17 févr. 2020
 *      Author: Julien
 */

#ifndef SPI_H_
#define SPI_H_

/**************************************
 *		INCLUDES
 *************************************/
#include <stdint.h>
#include <stdbool.h>
#include "LPC176x.h"

/**************************************
 *		PUBLIC DEFENITIONS
 *************************************/


/**************************************
 *		PUBLIC ENUM
 *************************************/
typedef enum{
	_200KHz = 200000,
	_800KHz = 800000,
	_1MHz	= 1000000,
	_2MHz	= 2000000,
	_4MHz	= 4000000,
	_8MHz	= 8000000
}spiSpeed_e;
/**************************************
 * 		SPI FUNCTIONS
 *************************************/

	bool 		spi_init(void);

	uint8_t 	spi_8bitTransfert(uint8_t data);

	uint16_t 	spi_16bitTransfert(uint16_t data);

	uint32_t 	spi_32bitTransfert(uint32_t data);

	void 		spi_send80DummyClock(void);

	void 		spi_send32DummyClock(void);

	void 		spi_setSPIspeed(spiSpeed_e speed);

	uint16_t 	spi_giveSPIspeed(void); //in Hz

	bool 		spi_waitTimer(uint16_t mS);

	void 		spi_receiveMultiByte(BYTE *buffer,  UINT byteToReceive);

	void 		spi_sendMultiByte(const BYTE *buffer, UINT byteToSend);

	bool 		spi_waitTimerToken(uint16_t mS);

	bool 		spi_select(void);

	void 		spi_deselect(void);
#endif /* SPI_H_ */
