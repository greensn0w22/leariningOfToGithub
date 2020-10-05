/*
 * LPCfunctions.c
 *
 *  Created on: 4 sept. 2019
 *      Author: Surface-Julien
 */

#include "All.h"
char TBLPrint[8]= {'0','0','0','0','1','0','0','0'};

struct FlagsCMD CMD;//Déclarer Les flags de ligne de commande


void InitFlagsConst(){
/**************COMMAND LINE DESING***********
------------------------------------------------------------
|HEURE            			           |	 H:			   |
|							           |				   |
|CAPTURE FREQ/DUTY			           |FREQ:---- DUTY:---%|
|							           |				   |
|MOTEUR #1					           |ON/OFF:-  RPM:-----|
|							           |                   |
|MOTEUR #2					           |ON/OFF:-  RPM:-----|
|							           |				   |
|							           |				   |
|							           |                   |
|							           |                   |
|							           |                   |
|			  				           |                   |
------------------------------------------------------------
PROMPT>
LAST LPC INFO:
*/

	int x,y;

	CMD.ReadyToPrompt = 1;
	CMD.DownLine[0] = '\r';
	CMD.DownLine[1] = '\e';
	CMD.DownLine[2] = '[';
	CMD.DownLine[3] = 'B';

	CMD.UpperLine[0] = '\r';
	CMD.UpperLine[1] = '\e';
	CMD.UpperLine[2] = '[';
	CMD.UpperLine[3] = 'A';

	CMD.RightLine[0] = '\e';
	CMD.RightLine[1] = '[';
	CMD.RightLine[2] = 'C';

	CMD.LeftLine[0] = '\e';
	CMD.LeftLine[0] = '[';
	CMD.LeftLine[0] = 'D';

	CMD.Moteur = 0;
	CMD.SYSTickMoteur1 = 0;
	CMD.SYSTickMoteur2 = 0;
	CMD.CaptureOn = 0;
	CMD.Speed[0] = 50;
	CMD.Speed[1] = 50;
	CMD.FlagValidBoard = 0;
	CMD.IICFlag = 0;

	CMD.CPT1inc = 0;
	CMD.CPT1Tot = 1200;

	CMD.CPT2inc = 0;
	CMD.CPT2Tot = 400;


	for(y=0;y<16;y++)
	{
		for(x=0;x<50;x++)
		{
			if(y==0 || y== 14)
				CMD.TableauAffichage[y][x]='-';
			else if((x==0 || x==49 || x==28) && (y <= 14))
				CMD.TableauAffichage[y][x]='|';
			else
				CMD.TableauAffichage[y][x]=' ';
		}

	}

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			CMD.ClavMat[x][y] = 0;
		}

	}

	SegOff();


	WriteBoard("HEURE",2,HEURESCREEN);
	//WriteBoard("TEXTE:",36,AFFICHEUR);

	WriteBoard("GAZ #1 | GAZ #2",2,GAZ);
	WriteBoard("[    ]mL | [    ]mL",30,GAZ);

	WriteBoard("TEMPERATURE #1",2,TEMPERATURE1);
	WriteBoard("[  .  ]C [  . ]F",30,TEMPERATURE1);

	WriteBoard("TEMPERATURE #2",2,TEMPERATURE2);
	WriteBoard("[  .  ]C [  . ]F",30,TEMPERATURE2);

	WriteBoard("CAPTURE FREQ/DUTY",2,CAPTURE);
	WriteBoard("FREQ:-----",30,CAPTURE);
	WriteBoard("DUTY:---%",41,CAPTURE);

	WriteBoard("MOTEUR #1 O(1) +(2) -(3)",2,MOTEUR1);
	WriteBoard("O:",30,MOTEUR1);
	WriteBoard("RPM:----",42,MOTEUR1);

	WriteBoard("MOTEUR #2 O(5) +(6) -(7)",2,MOTEUR2);
	WriteBoard("O:",30,MOTEUR2);
	WriteBoard("RPM:----",42,MOTEUR2);

	WriteBoard("LIGNE1",2,COMPTEL1);
	WriteBoard("0000/1200",30,COMPTEL1);

	WriteBoard("LIGNE2",2,COMPTEL2);
	WriteBoard("0000/0400",30,COMPTEL2);

	WriteBoard("PROMPT>",1,PROMPT);

	WriteBoard("LAST LPC INFO:",1,INFOLPC);

	CMD.BitBucket = 0;




}

unsigned char decoderAlphaNum(char Caracter)
{

	int i;
	const int Longueur = 37;

	unsigned char TBLDecoder[37][2] = {
		{'0',0},
		{'1',1},
		{'2',2},
		{'3',3},
		{'4',4},
		{'5',5},
		{'6',6},
		{'7',7},
		{'8',8},
		{'9',9},
		{'a',10},
		{'b',11},
		{'c',12},
		{'d',13},
		{'e',14},
		{'f',15},
		{'g',16},
		{'h',17},
		{'i',18},
		{'j',19},
		{'k',20},
		{'l',21},
		{'m',22},
		{'n',23},
		{'o',24},
		{'p',25},
		{'q',26},
		{'r',27},
		{'s',28},
		{'t',29},
		{'u',30},
		{'v',31},
		{'w',32},
		{'x',33},
		{'y',34},
		{'z',35},
		{' ',48}
		};
	if ((Caracter > 0x40) && (Caracter < 0x5b))
			Caracter |= 0x20;

	for (i = 0; i<=Longueur; i++){

		if(Caracter == (TBLDecoder[i][0])){
			Caracter = TBLDecoder[i][1];
			break;
		}

		if(i == 36)
			Caracter = 48;
	}


	return Caracter;
}

void PrintAlphaNum(const char *PtrTexte){



	static unsigned char Counter = 0;
	unsigned char Value;
	//int i;




	Value = decoderAlphaNum(*(PtrTexte+Counter));



	FIO2PIN1 = ((FIO2PIN1&0xE3)|(Counter<<2));
	FIO2PIN0 = ((FIO2PIN0&0xC0)|Value);
	if(CMD.ActiveDot == Counter)
		FIO2PIN0 |= 0x40;
	else
		FIO2PIN0 &= 0xBF;





	if (Counter == 7)
		Counter = 0;
	else
		Counter ++;

}

void InitSYSTick(void){
	STCTRL = 7;
	STRELOAD = (960000-1)*5; //50ms
}


void ActiveRIT(unsigned char YN, unsigned int Microsec, char Divider)
{

	//SETUP LES GPIO
	FIO2DIRL |= 0x1C7F;

	if (YN){

		//active le power dans le mux
		SetBit(16,PCONP);

		switch (Divider)
		{
			case 4 :

				//set le 24MHz
				PCLKSEL1 = (PCLKSEL1&0xF9FFFFFF)|(0<<26);
				//Nombre de compte pour trig un match
				RI_TIMER_COMPVAL = ((unsigned int)(Microsec*24));
				RI_TIMER_COUNT = 0;
				break;

			case 1 :
				PCLKSEL1 = (PCLKSEL1&0xF9FFFFFF)|(1<<26);
				RI_TIMER_COMPVAL = ((unsigned int)(Microsec*96));
				RI_TIMER_COUNT = 0;
				break;

			case 2 :
				PCLKSEL1 = (PCLKSEL1&0xF9FFFFFF)|(2<<26);
				RI_TIMER_COMPVAL = ((unsigned int)(Microsec*48));
				RI_TIMER_COUNT = 0;
				break;

			case 8 :
				PCLKSEL1 = (PCLKSEL1&0xF9FFFFFF)|(3<<26);
				RI_TIMER_COMPVAL = ((unsigned int)(Microsec*12));
				RI_TIMER_COUNT = 0;
				break;

			default:
				break;
		}

		//RITCTRL interrupt, clear timer, enable in debug, timer enable
		RI_TIMER_CTRL = 0x000F;


		//active vecteur d'interruption
		SetBit(29,ISER0);


	}
	else{
		ClearBit(16,PCONP);
		ClearBit(29,ISER0);
	}
	IPR7 = 0x0000f800;
}

void ActiveTMR0CaptureFreq(char YN){
	static int once = 0;
	static int alreadydead = 0;

	if (YN && !once){
		SetBit(1,PCONP);

		//set le clk a 24MHz
		PCLKSEL0 = ((PCLKSEL0&0XFFFFFFF3));

		//active le compte dans le PC&TC
		SetBit(0,T0TCR);

		//incrémentation sur clock interne
		T0CTCR = 0x0;

		//Active la Fonction CAP0.0&CAP0.1 sur P1.26&P1.27
		PINSEL3 = ((PINSEL3&0xFE1FFFFF)|(0xF<<20));

		//Désactive les pull-up/pull-down
		PINMODE3 = ((PINMODE3&0xFF0FFFFF)|(0xA<<20));

		//Set open sink
		SetBit(26,PINMODE_OD1);
		SetBit(27,PINMODE_OD1);

		//CAP0.0 sur Falling edge + intetrupt enable & CAP0.1 sur Rising edge + interrupt disable
		T0CCR = 0x0000000E;

		//active le power dans le mux
		SetBit(1,PCONP);

		//Active le vecteur d'interruption de Timer0
		SetBit(1, ISER0);

		//IPR0 = FlipxBit(IPR0,0x02,19,5);
		if(!once){//désactiver la capture la première fois
		once = 1;
		CMD.CaptureOn = 0;

		//ActiveTMR0CaptureFreq(0);
		}


	}
	else if(YN){
		SetBit(1,PCONP);
		SetBit(1,ISER0);
		SB(2,T0CCR);
		alreadydead = 0;

	}
	else{
		if (!alreadydead){
		SetBit(4,T0IR);
		CB(2,T0CCR);
		ClearBit(1,PCONP);
		alreadydead = 1;
		}
	}

}

void FlushPrint()
{
	TBLPrint[0] = 0;
	TBLPrint[1] = 0;
	TBLPrint[2] = 0;
	TBLPrint[3] = 0;
	TBLPrint[4] = 0;
	TBLPrint[5] = 0;
	TBLPrint[6] = 0;
	TBLPrint[7] = 0;
}

void ActivePWN(int Moteur1, int Moteur2, int YN){

	if (YN){
		//Active le module PWM
		SetBit(6,PCONP);

		//set le timer à 24MHz
		PCLKSEL0 = ((PCLKSEL0&0xFFFFCFFF)|(0x0<<12));

		//mode compteur
		PWM1CTCR = FlipxBit(PWM1CTCR, 0x0, 0, 2);

		//PWM 1.1 sur P1.18, PWM 1.3 sur P1.20
		PINSEL3 = FlipxBit(PINSEL3,2,4,2);
		PINSEL3 = FlipxBit(PINSEL3,2,10,2);

		//P1.28 IO Moteur 1
		PINSEL3 = FlipxBit(PINSEL3,0,24,2);

		//P1.29 IO Moteur 2
		PINSEL3 = FlipxBit(PINSEL3,0,26,2);

		//Met les 2 moteur à off
		FIO1DIR |= 0x30000000;
		FIO1PIN |= 0x30000000;

		//active le reset + enable counter + enable pwm
		PWM1TCR = 0x02;

		//set les pins 0.23 à 0.25 en GPIO out et ensuite a 0
		//pour selection du mux
		PINSEL1 &= (~0x3f);
		FIO0DIR |= 0x00070000;
		FIO0PIN |= 0x00070000;
		Sample(4);
		WriteBoard("D:----",36,MOTEUR2);
		UpdateBoardLine(MOTEUR2);

		//Active le output du pwm1
		PWM1PCR = FlipxBit(PWM1PCR, 0x5, 9, 3);


		//Valeur dans le MR
		PWM1MR0 = 961;
		//pinPWM1
		PWM1MR1 = 481;
		PWM1MR3 = 481;

		//PWM1LER charge le shadow
		PWM1LER = 0x7F;



		//désactive le reset
		PWM1TCR = 0x09;

	}
	if (!Moteur1){
		//ON a 0
		FIO1SET |= 0x10000000;
		WriteBoard("OFF",32,MOTEUR1);
		WriteBoard("RPM:----",42,MOTEUR1);
		WriteBoard("D:----",36,MOTEUR1);
	}
	else{
		FIO1CLR |= 0x10000000;
		WriteBoard("ON ",32,MOTEUR1);
		WriteBoard("RPM:",42,MOTEUR1);
		WriteBoard("D:",36,MOTEUR1);
	}

	UpdateBoardLine(MOTEUR1);

	if (!Moteur2){
		FIO1SET |= 0x20000000;
		WriteBoard("OFF",32,MOTEUR2);
		WriteBoard("RPM:----",42,MOTEUR2);
		WriteBoard("D:----",36,MOTEUR2);
	}
	else{
		FIO1CLR |= 0x20000000;
		WriteBoard("ON ",32,MOTEUR2);
		WriteBoard("RPM:",42,MOTEUR2);
		WriteBoard("D:",36,MOTEUR2);
	}

	UpdateBoardLine(MOTEUR2);

}

unsigned int FlipxBit(unsigned int Vecteur,unsigned int BitValue,unsigned int PositionLSB,unsigned int NbBits){
	uint32_t Bits;
	for(; NbBits > 0; NbBits --)
		Bits = ((Bits<<1)|0x1);

	(Vecteur) = ((~((~(Vecteur))|(Bits<<PositionLSB)))|(BitValue<<PositionLSB));

	return Vecteur;
}

void EnableUart2(int YN){
	if(YN){
		CMD.ReadyToPrompt = 1;
		CB(3,PCONP);
		CB(4,PCONP);
		CB(25,PCONP);
		SB(24,PCONP);

		PCLKSEL1 = FlipxBit(PCLKSEL1, 0x1, 16, 2); //96MHz 460800bit/sec


		//metre les pin du uart à 0
		PINSEL0 = FlipxBit(PINSEL0, 0x00, 0, 4);
		PINSEL9 = FlipxBit(PINSEL9, 0x00, 24, 4);
		PINSEL0 = FlipxBit(PINSEL0, 0x00, 20, 4);

		PINSEL4 = FlipxBit(PINSEL4, 0xA, 16, 4);

		//PINMODE4 = FlipxBit(PINMODE4,0x0A,16,4); //open drain

		U2LCR = 0x80;
		U2DLL = 13;
		U2DLM = 0;
		U2LCR = 0x00; //BaudRateConfig

		U2FDR = 0x10; //DivVal =0 | MulVal = 1

		U2LCR = 0x03;

		U2FCR = 0x6; //Disable FIFO + Clear FIFO

		U2TER |= 0x80;

		//SB(2,U2IER);
		SB(0,U2IER);
		CMD.CommandLinePos = 0;
		//fin
		SB(7,ISER0);
		IPR2 = FlipxBit(IPR2, 0x1E, 27, 5);

	}


	else{
		CB(24,PCONP);
	}
}

void CommandLine(void){
	int i;

	PINSEL4 = FlipxBit(PINSEL4,0x0,18,2);//disable RX during command line
	CMD.FlagInterrupt = 0;
	DivideCommands();

	if(CMD.NumberOfOptions)
		Level1();

	//Clean command line
	for(i=0;i<43;i++)
	CMD.CommandLine[i] = 0;

	for(i=0;i<25;i++)
		CMD.OptionA[i] = 0;

	for(i=0;i<25;i++)
		CMD.OptionB[i] = 0;

	for(i=0;i<20;i++)
		CMD.OptionC[i] = 0;

	for(i=0;i<10;i++)
		CMD.OptionD[i] = 0;

	CMD.NumberOfOptions = 0;
	PINSEL4 = FlipxBit(PINSEL4,0x2,18,2);//enable RX again
}

void CommandLineWrite(const char* Pointer, int SizeOfString){
	int i;

	while(!(U2LSR&0x20))
	{}
	//Si c'est une string Size of String = 0
	if (!SizeOfString)
	{
		i=0;
		while(*(Pointer+i))
		{
			U2THR = *(Pointer+i);
			i++;
		}
	}
	//Si c'est un tableau, la largeur doit être spécifiée
	else{
		for(i=0; i<SizeOfString; i++){
			U2THR = *(Pointer+i);
		}
	}
}

void ClrScreen(void){
	int i;
	int j;
	PINSEL4 = FlipxBit(PINSEL4,0x0,18,2);//disable RX during clear

	for(i=0;i<17;i++)
		CommandLineWrite(CMD.UpperLine,4);

	for(i=0;i<17;i++)
	{
		CommandLineWrite("\r",0);
		for(j=0;j<50;j++)
			CommandLineWrite(" ",0);
		CommandLineWrite("\n",0);
	}

	CommandLineWrite("\r",0);
	PINSEL4 = FlipxBit(PINSEL4,0x2,18,2);//enable RX again

}

/*****************************************
 * ÉCRIT A UN ENDROIT PARTICULIER DU BOARD
 * X à partir de 1
 * Y à partir de 0
 *
 *****************************************/
void WriteBoard(const char* ptr,int x, int y)
{
	int i=0;
	x -= 1;
	while(*(ptr+i)){
		CMD.TableauAffichage[y][x+i] = *(ptr+i);
		i++;
	}
	//UpdateBoardLine(y);
}

/**************************
 * Ligne à partir de 0
 *
 */
void UpdateBoardLine(int y){
	int i;

	CMD.ReadyToPrompt = 0;
	for(i=0;i<17;i++)
		CommandLineWrite(CMD.UpperLine,4);
	for(i=0;i<y;i++)
		CommandLineWrite(CMD.DownLine,4);
	for(i=0;i<50;i++)
	{
		while(!(U2LSR&0x20))
		{}
		U2THR = CMD.TableauAffichage[y][i];
	}
	ReturnToPrompt();
	CMD.ReadyToPrompt = 1;
}

void UpdateAllBoard(void){
	int x,y;

	for(x=0;x<17;x++)
		CommandLineWrite(CMD.UpperLine,4);

	for(y=0;y<17;y++){
		for(x=0;x<50;x++){
			while(!(U2LSR&0x20))
			{}
			U2THR = CMD.TableauAffichage[y][x];

		}
		while(!(U2LSR&0x20))
		{}
		U2THR = '\n';
		while(!(U2LSR&0x20))
		{}
		U2THR = '\r';
	}
	ValidBoard();

}

void ValidBoard(void){
	int i;
	static int Once = 0;

	if (Once)
	CMD.FlagInterrupt = 1;
	else
		Once++;

	UpdateBoardLine(PROMPT);
	CommandLineWrite("\r",0);
	for (i=0;i<7;i++)
	CommandLineWrite(CMD.RightLine,3);



}

void DivideCommands(void){

	int i;
	int KeepCount=0;
	CMD.NumberOfOptions = 0;
	//update première partie de la commande
	if(CMD.CommandLine[0] || (CMD.CommandLine[0]!= ' ')){
		for(i=0;i<25;i++)
		{
			if((CMD.CommandLine[i] == 0) ||(CMD.CommandLine[i] == ' '))
			{
				CMD.OptionA[i] = 0;
				KeepCount = i+1;

				CMD.NumberOfOptions = 1;
				break;
			}
			else
			CMD.OptionA[i] = CMD.CommandLine[i];
		}

		//optionB
		for(i=0;i<25;i++)
		{
			if((CMD.CommandLine[i+KeepCount] == 0) || (CMD.CommandLine[i+KeepCount] == ' '))
			{
				CMD.OptionB[i] = 0;
				KeepCount = KeepCount+i+1;

				if(i){
				CMD.NumberOfOptions++;}

				break;
			}
			else
			 CMD.OptionB[i] = CMD.CommandLine[i+KeepCount];
		}

		//optionC
		if(CMD.NumberOfOptions == 2)
		{
			for(i=0;i<25;i++){
				if((CMD.CommandLine[i+KeepCount] == 0) ||(CMD.CommandLine[i+KeepCount] == ' '))
				{
					CMD.OptionC[i] = 0;
					KeepCount = KeepCount+i+1;

					if(i){
					CMD.NumberOfOptions++;}

					break;
				}
				else
				 CMD.OptionC[i] = CMD.CommandLine[i+KeepCount];
			}
		}
		if(CMD.NumberOfOptions == 3)
		{
			for(i=0;i<25;i++){
				if((CMD.CommandLine[i+KeepCount] == 0) ||(CMD.CommandLine[i+KeepCount] == ' '))
				{
					CMD.OptionD[i] = 0;
					KeepCount = KeepCount+i;

					if(i){
					CMD.NumberOfOptions++;}

					break;
				}
				else
				 CMD.OptionD[i] = CMD.CommandLine[i+KeepCount];
			}
		}
	}



}

void ReturnToPrompt(void){
	int i;

	for(i=0; i<17; i++){
		CommandLineWrite(CMD.UpperLine, 4);
	}
	for(i=0; i<15;i++){
		CommandLineWrite("\n",0);
	}
	for(i=0; i<(7+CMD.CommandLinePos); i++)
		CommandLineWrite(CMD.RightLine,3);
}

void LLI(const char *pointer){
	int i;
	for(i=0;i<50;i++)
		CMD.TableauAffichage[16][i] = ' ';
	WriteBoard("LAST LPC INFO:",1,INFOLPC);
	UpdateBoardLine(INFOLPC);
	WriteBoard(pointer,16,INFOLPC);
	UpdateBoardLine(INFOLPC);
}

int SegReturn(void){
	if(CMD.SEGFreq){
		return 1;
	}

	else if(CMD.SEGDuty){
		return 2;
	}

	else if(CMD.SEGMoteur1){
		return 3;
	}

	else if(CMD.SEGMoteur2){
		return 4;
	}

	else if(CMD.SEGFree){
		return 5;
	}

	else if(CMD.SEGMoteur1Duty){
		return 7;
	}
	else if(CMD.SEGMoteur2Duty){
		return 9;
	}
	else
		return 0;
}

void SegOff(void){
	CMD.SEGDuty = 0;
	CMD.SEGFree = 0;
	CMD.SEGFreq = 0;
	CMD.SEGMoteur1 = 0;
	CMD.SEGMoteur2 = 0;
	CMD.SEGMoteur1Duty = 0;
	CMD.SEGMoteur2Duty = 0;
	CMD.SEGTempC1 = 0;
	CMD.SEGTempF1 = 0;
	CMD.SEGTempC2 = 0;
	CMD.SEGTempF2 = 0;
	CMD.SEGHeure = 0;
	CMD.SEGGaz1 = 0;
	CMD.SEGGaz2 = 0;
}

void ChangeMSpeed(int Moteur){
	int MRx;
	int Keep[2];
	int i;
	int error = 0;
	for(i = 0; i<2; i++){
		if((CMD.OptionC[i] > 0x29) && (CMD.OptionC[i] < 0x3A)){
			Keep[i] = CMD.OptionC[i];
		}
		else{
			error = 1;
			break;
		}
	}

	if(!error){
		MRx = (((((Keep[0]-0x30)*10)+(Keep[1]-0x30))*961)/100);
		CMD.Speed[(Moteur-1)] = (((Keep[0]-0x30)*10)+(Keep[1]-0x30));
		if(Moteur == 1)
			PWM1MR1 = MRx;
		else
			PWM1MR3 = MRx;
		PWM1LER = 0x7F;
	}
	else{
		LLI("Error in speed");
	}

}

void Sample(int pin){
	int i;
	ActiveTMR0CaptureFreq(0);
	FIO0PIN = FlipxBit(FIO0PIN,pin,16,3);
	for(i=0; i<10;i++)
	{}
	if((pin == 4)&&(!CMD.CaptureOn) ){
		ActiveTMR0CaptureFreq(0);
	}
	ActiveTMR0CaptureFreq(1);


}

void InitKeyPad(void){
/*************************
 *  R1  0  0  0  0		 *
 *  R2  0  0  0  0		 *
 *  R3  0  0  0  0		 *
 *  R4  0  0  0  0		 *
 *     C1 C2 C3 C4		 *
 *                       *
 * C = Output OD pull up *
 * R = Input pull up     *
 *************************/
	//SET P0.0 - P0.7 EN GPIO
	PINSEL0 &= (~0x0000ffff);


	//C = 1 output, R = 0 input
	FIO0DIR0 = 0xF0;

	//répondre aux critères en haut
	PINMODE_OD0 |= 0xF0;
	PINMODE0 &= 0xFFFF0000;


	//C en Output LOW
	FIO0CLR0 = 0xF0;

	//setup les interrupts
	IO0IntEnF = 0x0000000F;


	IO0IntClr = 0x0000000f;

	SB(21,ISER0);
}

void UpSpeed(int Moteur){

	int MRx;
	if((CMD.Speed[Moteur-1] + 1)<96){
		CMD.Speed[Moteur-1] += 1;
		LLI("SPEED+1!");


	MRx = ((CMD.Speed[Moteur-1]*961)/100);

	if(Moteur == 1)
		PWM1MR1 = MRx;
	else
		PWM1MR3 = MRx;
	}
	else{
		LLI("Speed already too high!");
	}
	PWM1LER = 0x7F;

}
void DownSpeed(int Moteur){

	int MRx;
	if((CMD.Speed[Moteur-1] - 1)>0){
		CMD.Speed[Moteur-1] -= 1;
		LLI("SPEED-1!");


	MRx = ((CMD.Speed[Moteur-1]*961)/100);

	if(Moteur == 1)
		PWM1MR1 = MRx;
	else
		PWM1MR3 = MRx;
	}
	else{
		LLI("Speed already too LOW!");
	}
	PWM1LER = 0x7F;
}

void ActiveI2C2(int YN){

	if(YN){
		//Active L'I2C2
		SB(26,PCONP);
		CB(7,PCONP);
		CB(19,PCONP);

		//Vit 12MHz
		PCLKSEL1 = FlipxBit(PCLKSEL1,0x3,20,2);

		//12MHz / (SCLL+SCHL) = 40KHz
		I2C2SCLL = 150;
		I2C2SCHL = 150;

		//PIN 0.10 SDA2 & 0.11 SCL2
		PINSEL0 = FlipxBit(PINSEL0, 0xA, 20,4);
		//PINS en OPEN DRAIN
		PINMODE_OD0 |= 0xC00;
		//active I2C
		SB(6,I2C2CONSET);
		//Active interrupt
		//SB(12,ISER0);

		char Tampon[3] = {0xFE,0x51,0x8};
		ClearLCD();
		Tampon[1] = 0x4c;
		SendIIC(LCD,Tampon,2);
		Tampon[1] = 0x53;
		SendIIC(LCD,Tampon,3);

		WriteLCD("Allo Diana",0,2);


	}
}




void SendIIC(int SlaveAdd, char* Data, int lenght){

	int i;

	I2C2CONCLR = 0x68;


	//Enable I2C + Start
	I2C2CONSET |= 0x60;

	//On attend le start
	while(I2C2STAT != 0x08){}
	//Clear SI
	I2C2CONCLR = 0x8;
	//Load l'addresse Slave
	I2C2DAT = SlaveAdd;
	I2C2CONCLR = 0x20;

	//l'addresse a été lue
	while((I2C2STAT != 0x18)){}
	I2C2CONCLR = 0x8;

	for(i=0; i<lenght; i++){

		I2C2DAT = *(Data+i);
		while((I2C2STAT != 0x28)){}
		I2C2CONCLR = 0x8;
	}

	//Send Stop
	I2C2CONSET |= 0x10;

}

void ClearLCD(void){
	char CLS[2] = {0xFE,0x51};
	char Home[3] = {0xFE,0x45,0x00};
	SendIIC(LCD,CLS,2);
	for(int i=0;i<1000;i++){}
	SendIIC(LCD,Home,3);
	for(int i=0;i<1000;i++){}
}

void WriteLCD(char* Text,int Pos,int Line){

	PosLCD(Pos,Line);

	SendIIC(LCD,Text,strlen(Text));


}

void PosLCD(int Pos,int Line){
	char POS[3] = {0xFE,0x45,0};

	switch (Line) {
	case 0:
		POS[2] = Pos;
		SendIIC(LCD,POS,3);
		break;
	case 1:
		POS[2] = (Pos+0x40);
		SendIIC(LCD,POS,3);
		break;
	case 2:
		POS[2] = (Pos+0x14);
		SendIIC(LCD,POS,3);
		break;
	case 3:
		POS[2] = (Pos+0x54);
		SendIIC(LCD,POS,3);
		break;
	default :
		break;
	}

}

void InitIIC(void){
//////////////////////////////////////////
	CMD.LCDMoteur1 = 1;
	CMD.LCDMoteur2 = 0;

	CMD.LCDTemp1 = 1;
	CMD.LCDTemp2 = 0;

	CMD.LCDCelsius = 1;
	CMD.LCDFareneith = 0;

	CMD.LCDHeure = 1;

	CMD.LCDGaz1 = 1;
	CMD.LCDGaz2 = 0;

	CMD.LCDCompte = 1;
	CMD.LCDCompte2 = 0;
//////////////////////////////////////////
/**********************************************
 * M1:1111RPM11Duty:11%
 * T1:11.11C11111111111
 * F:1111Hz1H[10:29:35]
 * C:111/12001Gaz1:1111
 * *******************************************/


	char LINE[21] = {0};
	sprintf(LINE,"M1:    RPM  Duty:  %");
	WriteLCD(LINE,0,0);

	sprintf(LINE,"T1:  .  C           ");
	WriteLCD(LINE,0,1);

	sprintf(LINE,"F:    Hz H[  :  :  ]");
	WriteLCD(LINE,0,2);

	sprintf(LINE,"C:  0/1200 Gaz1:    ");
	WriteLCD(LINE,0,3);




}

void ActiveRTC(void){
	SB(9,PCONP);

	RTC_CCR = 0x2;

	RTC_SEC = 0x0;

	RTC_MIN = 35;

	RTC_HOUR = 20;

	RTC_CCR = 0x1;


}

void ActiveCAN(){

	SB(12,PCONP);

	//pin 0.23-0.25 & 1.30 en ADC et FLOAT (AD0,1,2,4)
	PINSEL1 = ((PINSEL1&(~0x000FC000))|0x00054000);
	PINMODE1 = ((PINMODE1&(~0x000FC000))|0x000A8000);

	PINSEL3 |= 0x30000000;
	PINMODE3 = ((PINMODE3&(~0x30000000))|0x20000000);

	AD0INTEN = 0;
	AD0CR = 0x210017;

}

void ActiveDAC(void){
	PINSEL1 = ((PINSEL1&(~0x00C00000))|0x00800000);
}
