/*
 * LPCfunctions.h
 *
 *  Created on: 4 sept. 2019
 *      Author: Surface-Julien
 */

#define HEURESCREEN	1
#define CAPTURE		3
#define MOTEUR1		5
#define MOTEUR2		6
#define PROMPT		15
#define INFOLPC		16
#define GAZ		    8
#define TEMPERATURE1		10
#define TEMPERATURE2		11
#define COMPTEL1	12

#define COMPTEL2	13


#define LCD			0x50

typedef unsigned char byte;





#include "stdint.h"

#ifndef LPC_FUNCTIONS
#define LPC_FUNCTIONS

struct FlagsCMD{

	int FlagInterrupt;
	int ReceivedData;

	unsigned char CommandLine[43];
	int CommandLinePos;
	char OptionA[25];
	char OptionB[25];
	char OptionC[20];
	char OptionD[10];
	int OptionTotal;
	int NumberOfOptions;

	int FallBack;


	unsigned char TableauAffichage[17][50];
	unsigned char ClavMat[4][4];
	int TabX;
	int TabY;
	int BitBucket;

	char UpperLine[4];
	char DownLine[4];
	char LeftLine[3];
	char RightLine[3];

	char FREQDUTY[20]; //20 doit toujours être 0
	char SMOTEUR[5];//5 doit toujours être 0


	int ActiveDot;

	int CaptureOn;

	int FlagValidBoard;



	int CommandValue;

	int ReadyToPrompt;

	int DoneWithEngine;

	int SYSTickCapteur;
	int SYSTickMoteur1;
	int SYSTickMoteur2;
	int SYSTickMoteur1Duty;
	int SYSTickMoteur2Duty;
	int SYSTickExtInt;
	int SYSTickTrigUp;
	int SYSTickTrigDown;

	int Moteur;
	int Speed[2];

	int SEGFreq;
	int SEGDuty;
	int SEGMoteur1;
	int SEGMoteur2;
	int SEGFree;
	int SEGMoteur1Duty;
	int SEGMoteur2Duty;
	int SEGTempC2;
	int SEGTempF2;
	int SEGTempC1;
	int SEGTempF1;
	int SEGHeure;
	int SEGGaz1;
	int SEGGaz2;




	int IICFlag;
	int IICnew;
	byte IICSlaveAdd;

//////de base : Moteur1, T1, Celsius, Freq, Heure, Compte, Gaz1

	int LCDMoteur1;
	int LCDMoteur2;

	int LCDTemp1;
	int LCDTemp2;

	int LCDCelsius;
	int LCDFareneith;

	int LCDHeure;

	int LCDGaz1;
	int LCDGaz2;

	int LCDCompte;
	int LCDCompte2;
///////////////////////////////////////////////////////////////////

	int CPT1inc;
	int CPT1Tot;

	int CPT2inc;
	int CPT2Tot;





};

unsigned char decoderAlphaNum(char Caracter);

void PrintAlphaNum(const char *Texte);

void ActiveRIT(unsigned char YN, unsigned int Microsec, char Divider);

void ActiveTMR0CaptureFreq(char YN);

void FlushPrint(void);

void ActivePWN(int Moteur1, int Moteur2, int YN);

void ActiveI2C2(int YN);

void ActiveRTC(void);

void ActiveCAN(void);

void ActiveDAC(void);



/**************************************
 * @Vecteur
 * @BitValue
 * @PositionLSB
 * @NbBits
 **************************************/
unsigned int FlipxBit(unsigned int Vecteur,unsigned int BitValue,unsigned int PositionLSB,unsigned int NbBits);

void EnableUart2(int YN);

void CommandLine(void);

void InitFlagsConst(void);

void InitSYSTick(void);

void InitKeyPad(void);

void CommandLineWrite(const char *, int SizeOfString);

void ClrScreen(void);

void WriteBoard(const char *,int X, int Y);

void UpdateBoardLine(int y);

void UpdateAllBoard(void);

/****************************
 * LORSQUE ENTER EST APPUYÉ *
 ****************************/
void ValidBoard(void);

void DivideCommands(void);

void ReturnToPrompt(void);



void LLI(const char *pointer);

/**********************
 * Shutdown tous
 * les segments
 *********************/
void SegOff(void);


/*********************
 * 0 = aucun segment
 * 1 = SegFreq
 * 2 = SegDuty
 * 3 = SegMoteur1
 * 4 = SegMoteur2
 * 5 = SegFree
 * 7 = SEGMoteur1Duty
 * 9 = SEGMoteur2Duty
 ********************/
int SegReturn(void);

void ChangeMSpeed(int Moteur);

//change le sample du lecteur de fréquence
void Sample(int pin);

void UpSpeed(int Moteur);
void DownSpeed(int Moteur);

void ClearLCD(void);

void WriteLCD(char* Text,int Pos,int Line);

void PosLCD(int Pos,int Line);

void SendIIC(int SlaveAdd, char* Data, int lenght);

void InitIIC(void);



#endif /* LPC_FUNCTIONS */
