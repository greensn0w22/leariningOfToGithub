/*
 * mmc.c
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

#include "stdint.h"
typedef uint32_t		DWORD;	/* 32-bit unsigned integer */
typedef uint16_t		WORD;		/* UTF-16 character type */
/**************************************
 *			INCLUDES
 *************************************/

#include "stdbool.h"
#include "spi.h"
#include "diskio.h"
#include "LPC176x.h"
#include "error.h"

/**************************************
 *		PRIVATE DEFENITION
 *************************************/
//COMMAND SET FOR SD CARDS (0x40 + index) please refer to
//**http://elm-chan.org/docs/mmc/mmc_e.html#spimode** for
//more info

#ifdef __cplusplus
extern "C" {
#endif

#define CMD0		(0  | 0x40)
#define CMD1		(1  | 0x40)
#define CMD8		(8  | 0x40)
#define CMD9		(9  | 0x40)
#define CMD10		(10 | 0x40)
#define CMD12		(12 | 0x40)
#define CMD16		(16 | 0x40)
#define CMD17		(17 | 0x40)
#define CMD18		(18 | 0x40)
#define CMD23		(23 | 0x40)
#define CMD24		(24 | 0x40)
#define CMD25		(25 | 0x40)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32		(32 | 0x40)		/* ERASE_ER_BLK_START */
#define CMD33		(33 | 0x40)		/* ERASE_ER_BLK_END */
#define CMD38		(38 | 0x40)		/* ERASE */
#define	CMD48		(48 | 0x40)		/* READ_EXTR_SINGLE */
#define	CMD49		(49 | 0x40)		/* WRITE_EXTR_SINGLE */
#define CMD55		(55 | 0x40)
#define CMD58		(58 | 0x40)

#define ACMD13		(13 | 0xC0)		//NEED A CMD55 BEFORE
#define ACMD23		(23 | 0xC0)		//NEED A CMD55 BEFORE
#define ACMD41		(41 | 0xC0)		//NEED A CMD55 BEFORE


#define GO_IDLE_STATE 			CMD0 	//Software reset.
#define SEND_OP_COND			CMD1	//Initiate initialization process.
#define APP_SEND_OP_COND		ACMD41 	//For only SDC. Initiate initialization process.
#define SEND_IF_COND			CMD8	//For only SDC V2. Check voltage range.
#define SEND_CSD				CMD9	//Read CSD register.
#define SEND_CID				CMD10	//Read CID register.
#define STOP_TRANSMISSION		CMD12	//Stop to read data.
#define SET_BLOCKLEN 			CMD16	//Change R/W block size.
#define READ_SINGLE_BLOCK		CMD17	//Read a block.
#define READ_MULTIPLE_BLOCK		CMD18	//Read multiple blocks.
#define	SET_BLOCK_COUNT			CMD23	//For only MMC. Define number of blocks to transfer
										//with next multi-block read/write command.
#define SET_WR_BLOCK_ERASE_COUNT ACMD23 //	For only SDC. Define number of blocks to pre-erase
										//with next multi-block write command.
#define WRITE_BLOCK				CMD24   //Write a block.
#define WRITE_MULTIPLE_BLOCK 	CMD25	//Write multiple blocks.
#define APP_CMD					CMD55	//Leading command of ACMD<n> command.
#define READ_OCR				CMD58	//Read OCR.

//pdrv possibilities
#define MMC_CARD 0

//others declarations
#define CRC		0x01
#define ERR_R1	0x0

/**************************************
 *		PRIVATE DECLARATION
 *************************************/
BYTE DiskState = STA_NOINIT;
BYTE CardType  = 0;

static bool mmc_init(void);
static BYTE mmc_R1(void);
static BYTE mmc_indexCMD(BYTE cmd, uint32_t argument);
static bool mmc_receiveDtBlock(BYTE *buff, UINT DtBlckLeng);
static bool mmc_transmitDtBlock(const BYTE *buff, BYTE token);

/**************************************
 *		PRIVATE STRUCTURE
 *************************************/

/**************************************
 *		PUBLIC	FUNCTIONS
 *************************************/
DSTATUS disk_initialize (
	BYTE pdrv	/* physical drive */
){
	switch (pdrv) {
		case MMC_CARD :
			if (mmc_init() == STA_OK){
			DiskState = STA_OK;
				return DiskState;
			}
			else{
				DiskState = STA_NOINIT;
				return DiskState;
			}
		default :
			return DiskState;
	}

	return STA_NOINIT;
}

DSTATUS disk_status (
	BYTE pdrv	/* physical drive */
){
	if(pdrv != MMC_CARD) return STA_NOINIT;
	return DiskState;
}

DRESULT disk_read (
	BYTE pdrv, 		/* Physical drive number (0) */
	BYTE* buff, 	/* Pointer to the data buffer to store read data */
	LBA_t sector, 	/* Start sector number (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	BYTE cmd;
	DWORD sect = (DWORD)sector;

	if((pdrv != 0) || (count == 0)) return RES_PARERR;  /*check parameter*/
	if(DiskState & STA_NOINIT) 	return RES_NOTRDY;	/* Check drive status */

	if(!(CardType & CT_BLOCK)) sect *= 512; /*Convert block-type SD card to byte*/

	if(count > 1) cmd = CMD18; 	//READ_MULTIPLE BLOCK
	else cmd = CMD17;			//READ_SINGLE BLOCK

	if(mmc_indexCMD(cmd, sect) == 0x0){
		do{
			if(!mmc_receiveDtBlock(buff, 512)) break;
			buff += 512;
			count --;
		}while (count);
		if (cmd == CMD18) mmc_indexCMD(CMD12, 0); /* stop transmission if CMD18 */
	}

	spi_deselect();

	if(count != 0) return RES_ERROR;
	return RES_OK;
}

DRESULT disk_write (
	BYTE pdrv,
	const BYTE* buff,
	LBA_t sector,
	UINT count
)
{
	DWORD sect = (DWORD)sector;

	if (pdrv != 0 || count == 0) return RES_PARERR; /* Check parameter */
	if (DiskState & STA_NOINIT)  return RES_NOTRDY;	/* Check drive status */
	if (DiskState & STA_PROTECT) return RES_WRPRT;	/* Check write protect */

	if(!(CardType & CT_BLOCK)) sect *= 512; /*Convert block-type SD card to byte*/

	if(count == 1) /* Single sector write */
	{
		 if((mmc_indexCMD(CMD24, sect) == 0)&& mmc_transmitDtBlock(buff, 0xFE))	/* WRITE_BLOCK */
		 {
			count = 0;
		 }
	}
	else
	{
		if(mmc_indexCMD(CMD25, sect) == 0) /* Write multiple block */
		{
			do{
				if(!mmc_transmitDtBlock(buff, 0xFC)) break;

				buff += 512;
				count --;
			} while(count);
			if(!mmc_transmitDtBlock(0, 0xFD)) count = 1;    /* Stop_trans token */
		}
	}

	spi_deselect();

	if(count != 0) return RES_ERROR;
	return RES_OK;
}

DRESULT disk_ioctl (
	BYTE pdrv,
	BYTE cmd,
	void* buff
)
{
	DRESULT res = RES_ERROR;
	BYTE n, csd[16], *ptr = buff;
	DWORD st, ed, csize;
	LBA_t *dp;

	if (pdrv != 0) return RES_PARERR;				/* Check parameter */
	if (DiskState & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	switch(cmd){
		case CTRL_SYNC:				/* wait for internal write process of the drive */
			if(spi_select()) res = RES_OK;
			break;

		case GET_SECTOR_COUNT:	/* Get drive capacity in unit of sector (DWORD) */
			if ((mmc_indexCMD(CMD9, 0) == 0) && mmc_receiveDtBlock(csd, 16))
			{
				if((csd[0] >> 6) == 1) /* SDC ver 2.00 */
				{
					csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
					*(LBA_t *)buff = csize << 10;
				}
				res = RES_OK;
			}
			break;

		case GET_BLOCK_SIZE:	/* Get erase block size in unit of sector (DWORD) */
			if (CardType & CT_SD2) {	/* SDC ver 2.00 */
				if (mmc_indexCMD(ACMD13, 0) == 0) {	/* Read SD status */
					spi_8bitTransfert(0xFF);
					if (mmc_receiveDtBlock(csd, 16)) {				/* Read partial block */
						for (n = 64 - 16; n; n--) spi_8bitTransfert(0xFF);	/* Purge trailing data */
						*(DWORD*)buff = 16UL << (csd[10] >> 4);
						res = RES_OK;
					}
				}
			}
			break;

		case CTRL_TRIM:		/* Erase a block of sectors (used when _USE_TRIM in ffconf.h is 1) */
			if (!(CardType & CT_SDC)) break;				/* Check if the card is SDC */
			if (disk_ioctl(pdrv, MMC_GET_CSD, csd)) break;	/* Get CSD */
			if (!(csd[0] >> 6) && !(csd[10] & 0x40)) break;	/* Check if sector erase can be applied to the card */
			dp = buff; st = (DWORD)dp[0]; ed = (DWORD)dp[1];	/* Load sector block */
			if (!(CardType & CT_BLOCK)) {
				st *= 512; ed *= 512;
			}
			if (mmc_indexCMD(CMD32, st) == 0 && mmc_indexCMD(CMD33, ed) == 0 && mmc_indexCMD(CMD38, 0) == 0 && spi_waitTimer(30000)) {	/* Erase sector block */
				res = RES_OK;	/* FatFs does not check result of this command */
			}
			break;

		/* Following commands are never used by FatFs module */

		case MMC_GET_TYPE:		/* Get MMC/SDC type (BYTE) */
			*ptr = CardType;
			res = RES_OK;
			break;

		case MMC_GET_CSD:		/* Read CSD (16 bytes) */
			if (mmc_indexCMD(CMD9, 0) == 0 && mmc_receiveDtBlock(ptr, 16)) {	/* READ_CSD */
				res = RES_OK;
			}
			break;

		case MMC_GET_CID:		/* Read CID (16 bytes) */
			if (mmc_indexCMD(CMD10, 0) == 0 && mmc_receiveDtBlock(ptr, 16)) {	/* READ_CID */
				res = RES_OK;
			}
			break;

		case MMC_GET_OCR:		/* Read OCR (4 bytes) */
			if (mmc_indexCMD(CMD58, 0) == 0) {	/* READ_OCR */
				for (n = 4; n; n--) *ptr++ = spi_8bitTransfert(0xFF);
				res = RES_OK;
			}
			break;

		case MMC_GET_SDSTAT:	/* Read SD status (64 bytes) */
			if (mmc_indexCMD(ACMD13, 0) == 0) {	/* SD_STATUS */
				spi_8bitTransfert(0xFF);
				if (mmc_receiveDtBlock(ptr, 64)) res = RES_OK;
			}
			break;


		default:
			res = RES_PARERR;
	}

	spi_deselect();
	return res;
}
/**************************************
 *		PRIVATE	FUNCTIONS
 *************************************/
static bool mmc_init(void){
	uint32_t rx;

	spi_init();
	spi_send80DummyClock();
	FIO0CLR0 = (1 << 6);

	rx =  mmc_indexCMD(CMD0,0x00);

	if (rx == 0x01){    //SD card now in SPI MODE

		rx = mmc_indexCMD(CMD8, 0x1AA);
		if(rx == 0x01){
			rx = spi_32bitTransfert(0xFFFFFFFF); //looking for 0x1AA

			if((rx & 0x0fff) == 0x01AA){
				int counter = 0;

				do{
				rx =  mmc_indexCMD(ACMD41,0x40000000);
				counter++;
				if(rx > 1){ spi_deselect(); return STA_NOINIT;} //if there's an error break the init
				}	while(rx != 0x00);

				rx =  mmc_indexCMD(CMD58,0x00000000);
				if(rx == 0x00) rx = spi_32bitTransfert(0xFFFFFFFF); //read OCR CCS bit [30]
				else{ spi_deselect(); return STA_NOINIT;}

				if(rx & 0x40000000){ /* block type card */
					spi_setSPIspeed(_2MHz);
					CardType = CT_SD2|CT_BLOCK;
					spi_deselect();
					return STA_OK; //CCS == 1
				}
				else {
					rx =  mmc_indexCMD(CMD16,0x00000200); // force block size to 512 byte
					if(rx == 0x00) {
						spi_setSPIspeed(_2MHz);
						CardType = CT_SD2;
						spi_deselect();
						return STA_OK;
					}
					else return STA_NOINIT;
				}
			}
		}
	}
	else{
		//__set_PINSEL(0,6,2); 	//setup CS back to auto mode

		spi_deselect();

		return STA_NOINIT;
	}

	spi_deselect();
	return STA_NOINIT;
}

static BYTE mmc_indexCMD(BYTE cmd, uint32_t argument){
	BYTE response;  //whatever need to be sent back
	BYTE crc = 0x01;//always equals to 0x01



	if(cmd & 0x80){ //ACMD test
		spi_waitTimer(1000);
		cmd &= 0x7F;//remove ACMD encapsulation
		response = mmc_indexCMD(CMD55, 0x0);
		if(response != 0x01) return response;
		//spi_send32DummyClock(); //sending dummy clock for compatibility
	}

	if(cmd != CMD12){
		spi_deselect();
		if(!spi_select()) return 0xFF;
	}

	spi_8bitTransfert(cmd);
	spi_32bitTransfert(argument);

	if(cmd == CMD0) crc = 0x95;
	if(cmd == CMD8) crc = 0x87;
	spi_8bitTransfert(crc);

	if(cmd == CMD12){
		spi_8bitTransfert(0xFF); //send an end of transmition
	}

	response = mmc_R1();

	return response;
}

static BYTE mmc_R1()
{
	BYTE answer;
	uint8_t i = 0;
	do{
		answer = spi_8bitTransfert(0xFF);
		i++;
	}while((answer&0x80) && (i != 8));
	return answer;
}

static  bool mmc_receiveDtBlock(
	BYTE *buff,
	UINT DtBlckLeng
)
{
	if(!spi_waitTimerToken(500)) return 0;

	spi_receiveMultiByte(buff, DtBlckLeng);

	spi_16bitTransfert(0xFFFF);

	return 1;
}

static  bool mmc_transmitDtBlock(
		const BYTE *buff,
		BYTE token
)
{
	BYTE resp;

	if(!spi_waitTimer(500)) {return 0;}  	/* Check busy flag, wait for card ready to receive DtBlock */

	spi_8bitTransfert(token); 			/* Send token*/

	if(token == 0xFD) {return 1;}			/* Don't any data if token is a stop transmission*/

	spi_sendMultiByte(buff, 512); 		/* Data */

	spi_16bitTransfert(0xFFFF);			/* Dummy CRC */

	resp = spi_8bitTransfert(0xFF);		/* Receive data resp */

	if((resp & 0x1F) == 0x05) {return 1;} /* Data is accepted */
	else return 0;

	return 0;							/* Data has not been accepted */
}


#ifdef __cplusplus
extern }
#endif
