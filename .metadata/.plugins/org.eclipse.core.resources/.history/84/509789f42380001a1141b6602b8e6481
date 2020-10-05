/*
 * 3dprinter.h
 *
 *  Created on: 25 févr. 2020
 *      Author: Julien
 */

#ifndef PRINTER3DCLASS_H_
#define PRINTER3DCLASS_H_

#include <stdio.h>
#include "serialinterface.h"
#include "heat.h"
#include "motorclass.h"

extern "C" {
struct flag_t{
	bool flagGetHeatBlock:1;
	bool flagGetHeatBed:1;
	bool flagSerialProgress:1;
	bool flagSerialBlockTemp:1;
	bool flagSerialBedTemp:1;
	bool flagPIDBlock:1;
	bool flagPIDBed:1;
	bool endLoopFlag:1;
	bool SD:1;
	bool gcodeExecute:1;
	bool gCodeRead:1;

	uint32_t tenMillisecondSinceOn;
};
struct gcodeInfo_t {
	//FEED
	uint16_t FmmMinute;
	//TRAVEL NO EXTRUSION
	uint16_t TmmSec;
	//PRINTING
	uint16_t PmmSec;
	//POSITIONEMENT
	uint32_t X;
	uint32_t Y;
	uint32_t Z;
	uint32_t E;
	uint32_t ObjectiveX;
	uint32_t ObjectiveY;
	uint32_t ObjectiveZ;
	uint32_t ObjectiveE;
	//For the Home function
	bool homeWait;
	bool realPrint;
};
struct gCodeList_t{
	bool G0:1;
	bool G1:1;
	bool G28:1;
};
struct gCodeDecoder_t{
	uint8_t noOfParameter;
	char paramLetter[10];
	int16_t paramValue[10][2]; //pour simuler les chiffres à virgule
};

}


class printer3d {


	heat_c    	*heat;
	serialInt 	*Int;
	uint8_t 	heatBlockTemp;
	uint8_t 	heatBedTemp;
	motorsXYEZ 	mm;


/* public functions*/
public:
	printer3d();
	~printer3d();

	void printerMain(void);

/* private functions*/
private:
	void tempSeeker(void);
	void gcode(char* );
	void execute(void);
	void readSD(char* );
	void executeStep(uint8_t step);
	bool pixelDraw(bool g0g1, bool X, bool Y, bool Z, bool E);
	bool GList(gCodeDecoder_t* pointer);
	void g0g1Parameter(bool g0g1, gCodeDecoder_t* pointer);
};



#endif /* 3DPRINTERCLASS_H_ */
