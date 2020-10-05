/*
 * LPCCommand.c
 *
 *  Created on: 1 nov. 2019
 *      Author: Julien_School
 */

#include "LPCCommand.h"
//strcmp();

extern struct FlagsCMD CMD;
extern char TBLPrint[8];

void PutAllUpperCase(void){
	int i;

	for(i=0;i<25;i++)
	{
		if((CMD.OptionA[i]>0x60) && (CMD.OptionA[i]<0x7B)){
			CB(5,CMD.OptionA[i]);}
	}
	if(CMD.NumberOfOptions >= 2){
		for(i=0;i<25;i++)
		{
			if((CMD.OptionB[i]>0x60) && (CMD.OptionB[i]<0x7B)){
				CB(5,CMD.OptionB[i]);}
		}
	}
	if(CMD.NumberOfOptions >= 3){
		for(i=0;i<20;i++)
		{
			if((CMD.OptionC[i]>0x60) && (CMD.OptionC[i]<0x7B)){
				CB(5,CMD.OptionC[i]);}
		}
	}
	if(CMD.NumberOfOptions >= 4){
		for(i=0;i<25;i++)
		{
			if((CMD.OptionD[i]>0x60) && (CMD.OptionD[i]<0x7B)){
				CB(5,CMD.OptionD[i]);}
		}
	}


}

void Level1(void){
	PutAllUpperCase();

	if(!(strcmp(CMD.OptionA,"SEGMENT")))
			CMD.CommandValue = 0x1000;

	else if(!(strcmp(CMD.OptionA,"MOTEUR1")))
		CMD.CommandValue = 0x2000;

	else if(!(strcmp(CMD.OptionA,"MOTEUR2")))
			CMD.CommandValue = 0x4000;

	else if(!(strcmp(CMD.OptionA,"CAPTEUR")))
			CMD.CommandValue = 0x6000;

	else if(!(strcmp(CMD.OptionA,"LCD")))
			CMD.CommandValue = 0x8000;
	else if(!(strcmp(CMD.OptionA,"LINE")))
			CMD.CommandValue = 0x9000;

	else{
			CMD.CommandValue = 0xF000;
	}


	if(CMD.NumberOfOptions >=2)
		Level2();
	else
		Execute();
}

void Level2(void){

	if(CMD.CommandValue == 0x1000)
	{
		if(!(strcmp(CMD.OptionB,"SET")))
			CMD.CommandValue |= 0x0100;
		else if(!(strcmp(CMD.OptionB,"MODULE"))){
			CMD.CommandValue |= 0x0300;
		}
		else
		{
			CMD.CommandValue = 0xFF00; //ERREUR NIV 2
		}
	}

	else if(CMD.CommandValue == 0x2000)//Moteur 1
	{
		if(!(strcmp(CMD.OptionB,"ON")))
			CMD.CommandValue |= 0x0100;
		else if(!(strcmp(CMD.OptionB,"OFF"))){
			CMD.CommandValue |= 0x0200;
		}
		else if(!(strcmp(CMD.OptionB,"SPEED"))){
			CMD.CommandValue |= 0x0300;
			CMD.NumberOfOptions = 2;
		}
		else{
			CMD.CommandValue = 0xFF00; //ERREUR NIV 2
		}
	}

	else if(CMD.CommandValue == 0x4000)//Moteur 2
	{
		if(!(strcmp(CMD.OptionB,"ON")))
			CMD.CommandValue |= 0x0100;
		else if(!(strcmp(CMD.OptionB,"OFF"))){
			CMD.CommandValue |= 0x0200;
		}
		else if(!(strcmp(CMD.OptionB,"SPEED"))){
			CMD.CommandValue |= 0x0300;
			CMD.NumberOfOptions = 2;
		}
		else
		{
			CMD.CommandValue = 0xFF00; //ERREUR NIV 2
		}
	}


	else if(CMD.CommandValue == 0x6000) //capteur
	{
		if(!(strcmp(CMD.OptionB,"ON")))
			CMD.CommandValue |= 0x0100;

		else if(!(strcmp(CMD.OptionB,"OFF")))
			CMD.CommandValue |= 0x0200;

		else if(!(strcmp(CMD.OptionB,"SEGMENT"))){ //capteur segment
			CMD.CommandValue |= 0x0300;
		}
		else{
			CMD.CommandValue = 0xFF00; //ERREUR NIV 2
		}
	}

	else if(CMD.CommandValue == 0x8000) //LCD
	{
		if(!(strcmp(CMD.OptionB,"AFF")))
			CMD.CommandValue |= 0x0100;

		else{
			CMD.CommandValue = 0xFF00; //ERREUR NIV 2
		}
	}
	else if(CMD.CommandValue == 0x9000) //LCD
	{
		if(!(strcmp(CMD.OptionB,"1"))){
			CMD.CommandValue |= 0x0100;
			CMD.NumberOfOptions = 2;
		}
		else if(!(strcmp(CMD.OptionB,"2"))){
			CMD.CommandValue |= 0x0200;
			CMD.NumberOfOptions = 2;
		}


		else{
			CMD.CommandValue = 0xFF00; //ERREUR NIV 2
		}
	}


	if(CMD.NumberOfOptions >=3)
		Level3();
	else
		Execute();
}

void Level3(void){

	if((CMD.CommandValue&0xf000) == 0x1000) //SEGMENT
	{
		if((CMD.CommandValue&0x0f00) == 0x0100) //SET
		{
			if(!(strcmp(CMD.OptionC,"ALL")))
			{
				CMD.CommandValue |= 0x0010;
				CMD.NumberOfOptions = 3;
			}
			else
			{
				CMD.CommandValue = 0xFFF0;
			}

		}
		if((CMD.CommandValue&0x0f00) == 0x0300) //module
		{

			if(!(strcmp(CMD.OptionC,"MOTEUR1"))){
				CMD.CommandValue |= 0x0010;
			}
			else if(!(strcmp(CMD.OptionC,"MOTEUR1D"))){
				CMD.CommandValue |= 0x0020;
			}
			else if(!(strcmp(CMD.OptionC,"MOTEUR2"))){
				CMD.CommandValue |= 0x0030;
			}
			else if(!(strcmp(CMD.OptionC,"MOTEUR2D"))){
				CMD.CommandValue |= 0x0040;
			}
			else if(!(strcmp(CMD.OptionC,"TEMPC2"))){
				CMD.CommandValue |= 0x0050;
			}
			else if(!(strcmp(CMD.OptionC,"TEMPF2"))){
				CMD.CommandValue |= 0x0060;
			}
			else if(!(strcmp(CMD.OptionC,"TEMPC1"))){
				CMD.CommandValue |= 0x0080;
			}
			else if(!(strcmp(CMD.OptionC,"TEMPF1"))){
				CMD.CommandValue |= 0x0090;
			}
			else if(!(strcmp(CMD.OptionC,"HEURE"))){
				CMD.CommandValue |= 0x0070;
			}
			else if(!(strcmp(CMD.OptionC,"GAZ1"))){
				CMD.CommandValue |= 0x00A0;
			}
			else if(!(strcmp(CMD.OptionC,"GAZ2"))){
				CMD.CommandValue |= 0x00B0;
			}
			else{
				CMD.CommandValue = 0xFFF0;
			}

		}
		else{
			CMD.CommandValue = 0xFFF0;
		}

	}

	else if((CMD.CommandValue&0xf000) == 0x6000) //Capteur
	{
		if((CMD.CommandValue&0x0f00) == 0x0300) //SEGMENT
		{
			if(!(strcmp(CMD.OptionC,"FREQ")))
			{
				CMD.CommandValue |= 0x0010;


			}
			else if(!(strcmp(CMD.OptionC,"DUTY")))
			{
				CMD.CommandValue |= 0x0020;

			}
			else if(!(strcmp(CMD.OptionC,"OFF")))
			{
				CMD.CommandValue |= 0x0030;

			}

		}
	}

	else if((CMD.CommandValue&0xf000) == 0x8000) //
	{
		if((CMD.CommandValue&0x0f00) == 0x0100) //SEGMENT
		{
			if(!(strcmp(CMD.OptionC,"MOTEUR1"))){
				CMD.CommandValue |= 0x0010;
			}
			else if(!(strcmp(CMD.OptionC,"MOTEUR2"))){
				CMD.CommandValue |= 0x0020;

			}
			else if(!(strcmp(CMD.OptionC,"GAZ1"))){
				CMD.CommandValue |= 0x0030;

			}
			else if(!(strcmp(CMD.OptionC,"GAZ2"))){
				CMD.CommandValue |= 0x0040;

			}
			else if(!(strcmp(CMD.OptionC,"TEMP1C"))){
				CMD.CommandValue |= 0x0050;

			}
			else if(!(strcmp(CMD.OptionC,"TEMP1F"))){
				CMD.CommandValue |= 0x0060;

			}
			else if(!(strcmp(CMD.OptionC,"TEMP2C"))){
				CMD.CommandValue |= 0x0070;

			}
			else if(!(strcmp(CMD.OptionC,"TEMP2F"))){
				CMD.CommandValue |= 0x0080;

			}
			else{
				CMD.CommandValue = 0xFFF0;
			}

		}
	}

	else{
		CMD.CommandValue = 0xFFF0;
	}




	//if(CMD.NumberOfOptions == 4)
		//Level4();
	//else
		Execute();
}

void Execute(void){
	int i;
	//	AFFICHEUR
	if((CMD.CommandValue&0xf000) == 0x1000){
		switch (CMD.CommandValue) {
		case 0x1110 : //SEGMENT SET ALL
			SegOff();
			CMD.SEGFree = 1;
			for (i=0; i<8; i++)
				TBLPrint[i]= CMD.OptionD[i];
			LLI("SEGMENT SET ALL");
			break;
		case 0x1310 :
			SegOff();
			CMD.SEGMoteur1 = 1;
			break;
		case 0x1320 :
			SegOff();
			CMD.SEGMoteur1Duty = 1;
			break;
		case 0x1330 :
			SegOff();
			CMD.SEGMoteur2 = 1;
			break;
		case 0x1340 :
			SegOff();
			CMD.SEGMoteur2Duty = 1;
			break;
		case 0x1350 :
			SegOff();
			CMD.SEGTempC2 = 1;
			break;
		case 0x1360 :
			SegOff();
			CMD.SEGTempF2= 1;
			break;
		case 0x1370 :
			SegOff();
			CMD.SEGHeure = 1;
			break;
		case 0x1380 :
			SegOff();
			CMD.SEGTempC1 = 1;
			break;
		case 0x1390 :
			SegOff();
			CMD.SEGTempF1= 1;
			break;
		case 0x13A0 :
			SegOff();
			CMD.SEGGaz1= 1;
			break;
		case 0x13B0 :
			SegOff();
			CMD.SEGGaz2= 1;
			break;

		default :
			break;
		}
	}
	//MOTEUR 1
	else if((CMD.CommandValue&0xf000) == 0x2000){
		switch (CMD.CommandValue) {
		case 0x2100 :
			CMD.Moteur |= 0x1;
			ActivePWN(CMD.Moteur&0x1,CMD.Moteur&0x2,0);
			LLI("MOTEUR1 ON");
			if(CMD.LCDMoteur1){
				WriteLCD("M1:    RPM  Duty:  %",0,0);
				WriteLCD("F:    Hz",0,2);
			}
			break;

		case 0x2200 :
			CMD.Moteur &= ~0x1;
			ActivePWN(CMD.Moteur&0x1,CMD.Moteur&0x2,0);
			LLI("MOTEUR1 OFF");
			if(CMD.LCDMoteur1){
				WriteLCD("M1:    RPM  Duty:  %",0,0);
				WriteLCD("F:    Hz",0,2);
			}

			if(CMD.SEGMoteur1 || CMD.SEGMoteur1Duty){
				SegOff();
			}

			break;

		case 0x2300 :
			ChangeMSpeed(1);
			LLI("MOTEUR1 SPEED CHANGED!");
			break;

		case 0x2410 :
			UpSpeed(1);
			break;

		case 0x2420 :
			DownSpeed(1);
			break;

		default :
			break;
		}
	}

	//MOTEUR 2
	else if((CMD.CommandValue&0xf000) == 0x4000){
		switch (CMD.CommandValue) {
		case 0x4100 :
			CMD.Moteur |= 0x2;
			ActivePWN(CMD.Moteur&0x1,CMD.Moteur&0x2,0);
			LLI("MOTEUR2 ON");
			if(CMD.LCDMoteur2){
				WriteLCD("M2:    RPM  Duty:  %",0,0);
				WriteLCD("F:    Hz",0,2);
			}
			break;

		case 0x4200 :
			CMD.Moteur &= ~0x2;
			ActivePWN(CMD.Moteur&0x1,CMD.Moteur&0x2,0);
			LLI("MOTEUR2 OFF");
			if(CMD.LCDMoteur2){
				WriteLCD("M2:    RPM  Duty:  %",0,0);
				WriteLCD("F:    Hz",0,2);
			}

			if(CMD.SEGMoteur2 || CMD.SEGMoteur2Duty){
				SegOff();
			}
			break;

		case 0x4300 :
			LLI("MOTEUR2 SPEED CHANGED!");
			ChangeMSpeed(2);
			break;

		case 0x4410 :
			UpSpeed(2);
			break;

		case 0x4420 :
			DownSpeed(2);
			break;

		default :
			break;
		}
	}

	// CAPTEUR
	else if((CMD.CommandValue&0xf000) == 0x6000){
		switch (CMD.CommandValue) {
			case 0x6100 : //CAPTEUR ON
				CMD.CaptureOn = 1;
				Sample(4);
				LLI("CAPTURE ON");
				break;

			case 0x6200 : //CAPTEUR OFF
				ActiveTMR0CaptureFreq(0);
				CMD.SYSTickCapteur = 0;
				CMD.CaptureOn = 0;
				WriteBoard("FREQ:----- DUTY:---%",30,CAPTURE);
				UpdateBoardLine(CAPTURE);
				if((SegReturn() == 1) || SegReturn() == 2)
					SegOff();
				LLI("CAPTURE OFF");
				break;

			case 0x6310: //CAPTEUR SEGMENT FREQ
				SegOff();
				CMD.SEGFreq = 1;
				LLI("SEGMENT = FREQ");
				break;

			case 0x6320: //CAPTEUR SEGMENT DUTY
				SegOff();
				CMD.SEGDuty = 1;
				LLI("SEGMENT = DUTY");
				break;
			case 0x6330: //CAPTEUR SEGMENT DUTY
				SegOff();
				LLI("SEGMENT = OFF");
				break;


			default :
				break;
			}

	}

	else if((CMD.CommandValue&0xf000) == 0x8000){
		switch (CMD.CommandValue) {
			case 0x8110: // LCD AFF M1
				CMD.LCDMoteur1 = 1;
				CMD.LCDMoteur2 = 0;
				WriteLCD("M1:    RPM  Duty:  %",0,0);
				WriteLCD("F:    Hz",0,2);
				break;

			case 0x8120: // LCD AFF M1
				CMD.LCDMoteur1 = 0;
				CMD.LCDMoteur2 = 1;
				WriteLCD("M2:    RPM  Duty:  %",0,0);
				WriteLCD("F:    Hz",0,2);
				break;

			case 0x8130: // LCD AFF G1
				CMD.LCDGaz1 = 1;
				CMD.LCDGaz2 = 0;
				WriteLCD("Gaz1:    ",11,3);
				break;

			case 0x8140: // LCD AFF G2
				CMD.LCDGaz1 = 0;
				CMD.LCDGaz2 = 1;
				WriteLCD("Gaz2:    ",11,3);
				break;

			case 0x8150: // LCD AFF Temp1C
				CMD.LCDTemp1 = 1;
				CMD.LCDTemp2 = 0;
				CMD.LCDCelsius = 1;
				CMD.LCDFareneith = 0;
				break;

			case 0x8160: // LCD AFF Temp1F
				CMD.LCDTemp1 = 1;
				CMD.LCDTemp2 = 0;
				CMD.LCDCelsius = 0;
				CMD.LCDFareneith = 1;
				break;

			case 0x8170: // LCD AFF Temp2C
				CMD.LCDTemp1 = 0;
				CMD.LCDTemp2 = 1;
				CMD.LCDCelsius = 1;
				CMD.LCDFareneith = 0;
				break;

			case 0x8180: // LCD AFF Temp2F
				CMD.LCDTemp1 = 0;
				CMD.LCDTemp2 = 1;
				CMD.LCDCelsius = 0;
				CMD.LCDFareneith = 1;
				break;


			default :
				break;
			}

	}


	else if((CMD.CommandValue&0xf000) == 0x9000){
		char LINEE[10] = {0};
		char LINEEE[10] = {0};
		switch (CMD.CommandValue) {
			case 0x9100:

				WriteBoard("         ",30,COMPTEL1);
				CMD.CPT1Tot = atoi(CMD.OptionC);
				sprintf(LINEE,"%.4d/%.4d",CMD.CPT1inc,CMD.CPT1Tot);
				WriteBoard(LINEE,30,COMPTEL1);
				UpdateBoardLine(COMPTEL1);
				break;
			case 0x9200:

				WriteBoard("         ",30,COMPTEL2);
				CMD.CPT2Tot = atoi(CMD.OptionC);
				sprintf(LINEEE,"%.4d/%.4d",CMD.CPT2inc,CMD.CPT2Tot);
				WriteBoard(LINEEE,30,COMPTEL2);
				UpdateBoardLine(COMPTEL2);
				break;
			default:
				break;
		}

	}
	// ERREURS
	else{
		switch (CMD.CommandValue) {
			case 0xF000 :
				LLI("ERREUR ECRITURE NIV 1");
				break;

			case 0xFF00 :
				LLI("ERREUR ECRITURE NIV 2");
				break;

			case 0xFFF0 :
				LLI("ERREUR ECRITURE NIV 3");
				break;
			default :
				break;
			}
	}
}



