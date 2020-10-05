/*
 * interrupt.c
 *
 *  Created on: 10 sept. 2019
 *      Author: Julien
 */
#include "All.h"

extern char TBLPrint[8];
volatile char FreqDuty = 0;
extern struct FlagsCMD CMD;

void RIT_IRQHandler(void)
{
	SetBit(0,RI_TIMER_CTRL);

	if(CMD.SEGMoteur1Duty | CMD.SEGMoteur2Duty){
		CMD.ActiveDot = 5;
	}
	else if(CMD.SEGGaz1 | CMD.SEGGaz2){
		CMD.ActiveDot = 1;
	}
	else if(CMD.SEGTempC1 || CMD.SEGTempC2 || CMD.SEGTempF1 || CMD.SEGTempF2){
		CMD.ActiveDot = 4;
	}
	else{
		CMD.ActiveDot = 8;
	}
	PrintAlphaNum(TBLPrint);
}

void TIMER0_IRQHandler(void)
{
	static int compteur = 0;
	static int pwmSample = 0;
	static unsigned int T0 = 0;
	unsigned int T2,T1;
	unsigned int OutValue = 0,duty=0;
	int i;
	int fioright = 0;
	SetBit(4,T0IR);


	if(!CMD.SYSTickCapteur && !CMD.SYSTickMoteur2 && !CMD.SYSTickMoteur1 && !CMD.SYSTickMoteur2Duty && !CMD.SYSTickMoteur1Duty){
		if(!compteur)
		{
			T0 = T0CR0;
			compteur=1;
		}
		else{

			T2=T0CR0;
			T1=T0CR1;
			SetBit(1,T0TCR);
			ClearBit(1,T0TCR);
			fioright = ((FIO0PIN&0x00070000)>>16);
			//if(fioright==4){
			for(i=0;i<19;i++)
				CMD.FREQDUTY[i] = 0;

			if((fioright)==4){
				OutValue = 24000000/(T2-T0);
				duty = (((T2-T1)*100)/(T2-T0));

				if(OutValue<10000){
					sprintf(CMD.FREQDUTY,"%s%.4i%s%.3i%s","FREQ: ",OutValue," DUTY:",duty,"%");
					CMD.ActiveDot = 8;
				}
				else{
					sprintf(CMD.FREQDUTY,"%s%.2i%s%.1i%s%.3i%s","FREQ:",OutValue/1000,".",(OutValue%1000)/100,"K DUTY:",duty,"%");
					if(SegReturn() == 1)
						CMD.ActiveDot = 1;
				}
				CMD.SYSTickCapteur = 1; //data prete a être updaté
				compteur = 0;

			}

			else if((fioright)==1){
				compteur = 0;
				OutValue = ((24000000/(T2-T0))*30);

				pwmSample ++;
				if(pwmSample == 2){
					pwmSample = 0;
					CMD.SYSTickMoteur2 = 1;
					sprintf(CMD.FREQDUTY,"%.3d",(24000000/(T2-T0)));
					CMD.FREQDUTY[3] = 0;
					sprintf(CMD.SMOTEUR,"%.4i",OutValue);
				}
			}

			else if((fioright)==3){
				compteur = 0;
				OutValue = ((24000000/(T2-T0))*30);
				pwmSample ++;
				if(pwmSample == 2){
					pwmSample = 0;
					CMD.SYSTickMoteur1 = 1;
					sprintf(CMD.SMOTEUR,"%.4i",OutValue);
					sprintf(CMD.FREQDUTY,"%.3d",(24000000/(T2-T0)));
					CMD.FREQDUTY[3] = 0;
				}
			}
			else if((fioright)==0){
				duty = (((T2-T1)*10000)/(T2-T0));
				pwmSample ++;
				compteur = 0;
				if(pwmSample == 2){
					pwmSample = 0;
					CMD.SYSTickMoteur2Duty = 1;
					sprintf(CMD.SMOTEUR,"%.3i%s",duty/100,"%");
					sprintf(CMD.FREQDUTY,"%.4d",duty);
					CMD.FREQDUTY[4] = 0;
				}
			}
			else if((fioright)==2){
				duty = (((T2-T1)*10000)/(T2-T0));
				pwmSample ++;
				compteur = 0;
				if(pwmSample == 2){
					pwmSample = 0;
					sprintf(CMD.SMOTEUR,"%.3i%s",duty/100,"%");
					CMD.SYSTickMoteur1Duty = 1;
					sprintf(CMD.FREQDUTY,"%.4d",duty);
					CMD.FREQDUTY[4] = 0;
				}
			}
		}
	}
}






void UART2_IRQHandler(void){
	//pour empecher d'écrire en dehors du prompt
	while(!CMD.ReadyToPrompt){}
	while(CMD.FlagValidBoard){}

	CMD.ReceivedData = U2RBR;

	//13 = enter | 127 = BCK SPACE                           disable receive after '\e'
	if((CMD.ReceivedData == 13) || (CMD.ReceivedData == 127) || (CMD.BitBucket) || (CMD.ReceivedData == '\e'))
	{
		if((CMD.ReceivedData == 13))
		{
			CMD.CommandLine[CMD.CommandLinePos] = 0;
			CMD.FlagValidBoard = 1;
			//ValidBoard();
			CMD.CommandLinePos=0;
		}

		else if(CMD.ReceivedData == 127){
			if(CMD.CommandLinePos){
				U2THR = CMD.ReceivedData;
				CMD.CommandLinePos--;
				CMD.CommandLine[CMD.CommandLinePos] = 0;
			}
		}
		else{
			CMD.BitBucket++;
			if(CMD.BitBucket==3)
				CMD.BitBucket = 0;
		}

	}

	else{
		U2THR = CMD.ReceivedData;
		CMD.CommandLine[CMD.CommandLinePos] = CMD.ReceivedData;
		CMD.CommandLinePos++;
	}




}

//interrupts toutes les 500mS
void SysTick_Handler(void){

	static char i = 0;

	if(CMD.FlagValidBoard){
		CMD.FlagValidBoard = 0;
		ValidBoard();

	}


	if((i%4) == 0){
		static int Gaz1;
		static int Gaz2;
		char PRINT1[5] = {0};
		char PRINT2[5] = {0};
		Gaz1 = ((((AD0DR0&0xfff0)>>4)*3300)/4096);
		Gaz2 = ((((AD0DR1&0xfff0)>>4)*3300)/4096);
		sprintf(PRINT1,"%0.4d",Gaz1);
		sprintf(PRINT2,"%0.4d",Gaz2);

		if(CMD.LCDGaz1 && ((i%8) == 0)){

			WriteLCD(PRINT1,16,3);
			DACR = (((Gaz1*1024)/3300)<<6);

		}
		else if(CMD.LCDGaz2 && ((i%8) == 0)){

			WriteLCD(PRINT2,16,3);
			DACR = (((Gaz1*1024)/3300)<<6);

		}
		if(CMD.SEGGaz1){
			TBLPrint[0] = 'G';
			TBLPrint[1] = '1';
			TBLPrint[2] = PRINT1[0];
			TBLPrint[3] = PRINT1[1];
			TBLPrint[4] = PRINT1[2];
			TBLPrint[5] = PRINT1[3];
			TBLPrint[6] = 'M';
			TBLPrint[7] = 'L';

		}
		else if(CMD.SEGGaz2){
			TBLPrint[0] = 'G';
			TBLPrint[1] = '2';
			TBLPrint[2] = PRINT2[0];
			TBLPrint[3] = PRINT2[1];
			TBLPrint[4] = PRINT2[2];
			TBLPrint[5] = PRINT2[3];
			TBLPrint[6] = 'M';
			TBLPrint[7] = 'L';
		}

		if((i%8) == 4){
			sprintf(PRINT1,"%0.4d",Gaz1);
			sprintf(PRINT2,"%0.4d",Gaz2);
			WriteBoard(PRINT1,31,GAZ);
			WriteBoard(PRINT2,42,GAZ);
			UpdateBoardLine(GAZ);
		}


	}

	if((i%16) == 7){
		char LINE[12] = {0};
		if(CMD.LCDCompte){
			sprintf(LINE,"C:%.4d/%d",CMD.CPT1inc,CMD.CPT1Tot);
			WriteLCD("C:    /    ",0,3);
			WriteLCD(LINE,0,3);
		}
		else if(CMD.LCDCompte2){
			sprintf(LINE,"C:%.4d/%d",CMD.CPT2inc,CMD.CPT2Tot);
			WriteLCD("C:    /    ",0,3);
			WriteLCD(LINE,0,3);
		}

	}

	if((i&0x1)){
	//à chaque interrupt NEGATIF on update l'heure
		char HEURE[12] = {0};
		sprintf(HEURE,"H[%.2d:%.2d:%.2d]",RTC_HOUR,RTC_MIN,RTC_SEC);
		WriteLCD(HEURE,9,2);
		WriteBoard(HEURE,36,HEURESCREEN);
		UpdateBoardLine(HEURESCREEN);
		if(CMD.SEGHeure){
			TBLPrint[0] = HEURE[2];
			TBLPrint[1] = HEURE[3];
			TBLPrint[2] = 0;
			TBLPrint[3] = HEURE[5];
			TBLPrint[4] = HEURE[6];
			TBLPrint[5] = 0;
			TBLPrint[6] = HEURE[8];
			TBLPrint[7] = HEURE[9];

		}
	}

	//T1
	if((i%32)==3){
		int Conv;
		float Celsius,Fara;
		char CEL[6] = {0},FARA[5] = {0};

		Conv = ((((AD0DR2&0xfff0)>>4)*3300)/4096);
		Celsius = (float)(3300-Conv)/40;
		Fara = (Celsius*1.8)+32;

		sprintf(CEL,"%4.2f",Celsius);
		sprintf(FARA,"%3.1f",Fara);

		WriteBoard(CEL,31,TEMPERATURE1);
		WriteBoard(FARA,40,TEMPERATURE1);
		UpdateBoardLine(TEMPERATURE1);

		if(CMD.LCDTemp1 && CMD.LCDCelsius){
			WriteLCD("T1:  .  C",0,1);
			WriteLCD(CEL,3,1);
		}
		else if(CMD.LCDTemp1 && CMD.LCDFareneith){
			WriteLCD("T1:  . F ",0,1);
			WriteLCD(FARA,3,1);
		}
		if(CMD.SEGTempC1){
			TBLPrint[0] = 'T';
			TBLPrint[1] = '1';
			TBLPrint[2] = 0;
			TBLPrint[3] = CEL[0];
			TBLPrint[4] = CEL[1];
			TBLPrint[5] = CEL[3];
			TBLPrint[6] = CEL[4];
			TBLPrint[7] = 'C';
		}
		else if(CMD.SEGTempF1){
			TBLPrint[0] = 'T';
			TBLPrint[1] = '1';
			TBLPrint[2] = 0;
			TBLPrint[3] = FARA[0];
			TBLPrint[4] = FARA[1];
			TBLPrint[5] = FARA[3];
			TBLPrint[6] = 'F';
			TBLPrint[7] = 0;
		}


	}
	//T2
	else if((i%32)==19){
		int Conv;
		float Celsius,Fara;
		char CEL[6] = {0},FARA[5] = {0};

		Conv = ((((AD0DR4&0xfff0)>>4)*3300)/4096);
		Celsius = (float)(3300-Conv)/40;
		Fara = (Celsius*1.8)+32;

		sprintf(CEL,"%4.2f",Celsius);
		sprintf(FARA,"%3.1f",Fara);

		WriteBoard(CEL,31,TEMPERATURE2);
		WriteBoard(FARA,40,TEMPERATURE2);
		UpdateBoardLine(TEMPERATURE2);

		if(CMD.LCDTemp2 && CMD.LCDCelsius){
			WriteLCD("T2:  .  C",0,1);
			WriteLCD(CEL,3,1);
		}
		else if(CMD.LCDTemp2 && CMD.LCDFareneith){
			WriteLCD("T2:  . F ",0,1);
			WriteLCD(FARA,3,1);
		}
		if(CMD.SEGTempC2){
			TBLPrint[0] = 'T';
			TBLPrint[1] = '2';
			TBLPrint[2] = 0;
			TBLPrint[3] = CEL[0];
			TBLPrint[4] = CEL[1];
			TBLPrint[5] = CEL[3];
			TBLPrint[6] = CEL[4];
			TBLPrint[7] = 'C';
		}
		else if(CMD.SEGTempF2){
			TBLPrint[0] = 'T';
			TBLPrint[1] = '2';
			TBLPrint[2] = 0;
			TBLPrint[3] = FARA[0];
			TBLPrint[4] = FARA[1];
			TBLPrint[5] = FARA[3];
			TBLPrint[6] = 'F';
			TBLPrint[7] = 0;
		}



	}


	if((CMD.Moteur&0x2)&&(i == 20)){
		Sample(1);
		i++;
	}
	else if((CMD.Moteur&0x2)&&(i == 35)){
		Sample(0);
		i++;
	}
	else if((CMD.Moteur&0x1)&&(i == 50)){
		Sample(3);
		i++;
	}
	else if((CMD.Moteur&0x1)&&(i == 65)){
		Sample(2);
		i++;
	}
	else if(i == 66){
		i = 0;
	}
	else
		i++;

	if(CMD.SYSTickCapteur)
	{
			WriteBoard(CMD.FREQDUTY,30,CAPTURE);
			WriteBoard("|",50,CAPTURE);
			UpdateBoardLine(CAPTURE);
			CMD.SYSTickCapteur = 0;
			if(SegReturn() == 1){
				if(CMD.ActiveDot == 1){
					TBLPrint[0] = CMD.FREQDUTY[5];
					TBLPrint[1] = CMD.FREQDUTY[6];
					TBLPrint[2] = CMD.FREQDUTY[8];
					TBLPrint[3] = CMD.FREQDUTY[9];
					TBLPrint[4] = 0;
					TBLPrint[5] = 'H';
					TBLPrint[6] = 'Z';
					TBLPrint[7] = 0;
				}
				else{
					TBLPrint[0] = 0;
					TBLPrint[1] = CMD.FREQDUTY[6];
					TBLPrint[2] = CMD.FREQDUTY[7];
					TBLPrint[3] = CMD.FREQDUTY[8];
					TBLPrint[4] = CMD.FREQDUTY[9];
					TBLPrint[5] = 'H';
					TBLPrint[6] = 'Z';
					TBLPrint[7] = 0;
				}
			}
			else if(SegReturn() == 2){
				TBLPrint[0] = 'd';
				TBLPrint[1] = 'u';
				TBLPrint[2] = 't';
				TBLPrint[3] = 'y';
				TBLPrint[4] = 0;
				TBLPrint[5] = CMD.FREQDUTY[16];
				TBLPrint[6] = CMD.FREQDUTY[17];
				TBLPrint[7] = CMD.FREQDUTY[18];
			}
	}

	if(CMD.SYSTickMoteur1){
		CMD.SYSTickMoteur1 = 0;
		WriteBoard(CMD.SMOTEUR,46,MOTEUR1);
		UpdateBoardLine(MOTEUR1);
		Sample(4);
		if(SegReturn() == 3){
			TBLPrint[0] = 'R';
			TBLPrint[1] = 'P';
			TBLPrint[2] = 'M';
			TBLPrint[3] = 0;
			TBLPrint[4] = CMD.SMOTEUR[0];
			TBLPrint[5] = CMD.SMOTEUR[1];
			TBLPrint[6] = CMD.SMOTEUR[2];
			TBLPrint[7] = CMD.SMOTEUR[3];
		}
		if(CMD.LCDMoteur1){
			WriteLCD(CMD.SMOTEUR,3,0);
			WriteLCD(CMD.FREQDUTY,3,2);

		}

		//CMD.DoneWithEngine = 1;
	}


	if(CMD.SYSTickMoteur2){
		CMD.SYSTickMoteur2 = 0;
		WriteBoard(CMD.SMOTEUR,46,MOTEUR2);
		UpdateBoardLine(MOTEUR2);
		Sample(4);
		if(SegReturn() == 4){
			TBLPrint[0] = 'R';
			TBLPrint[1] = 'P';
			TBLPrint[2] = 'M';
			TBLPrint[3] = 0;
			TBLPrint[4] = CMD.SMOTEUR[0];
			TBLPrint[5] = CMD.SMOTEUR[1];
			TBLPrint[6] = CMD.SMOTEUR[2];
			TBLPrint[7] = CMD.SMOTEUR[3];
		}
		if(CMD.LCDMoteur2){
			WriteLCD(CMD.FREQDUTY,3,2);
			WriteLCD(CMD.SMOTEUR,3,0);

		}

		//CMD.DoneWithEngine = 1;
	}

	if(CMD.SYSTickMoteur2Duty){
		CMD.SYSTickMoteur2Duty = 0;
		WriteBoard(CMD.SMOTEUR,38,MOTEUR2);
		UpdateBoardLine(MOTEUR2);
		static int once = 0;
		if(!once){
			WriteBoard("D:----",36,MOTEUR2);
			UpdateBoardLine(MOTEUR2);
			once = 1;
		}
		Sample(4);
		if(SegReturn() == 9){
			TBLPrint[0] = 'D';
			TBLPrint[1] = 'M';
			TBLPrint[2] = '2';
			TBLPrint[3] = 0;
			TBLPrint[4] = CMD.FREQDUTY[0];
			TBLPrint[5] = CMD.FREQDUTY[1];
			TBLPrint[6] = CMD.FREQDUTY[2];
			TBLPrint[7] = CMD.FREQDUTY[3];
		}
		if(CMD.LCDMoteur2){
			char TBMoteur[10] = {0};
			sprintf(TBMoteur,"Duty:%s",(CMD.SMOTEUR+1));
			WriteLCD(TBMoteur,12,0);
		}
	}

	if(CMD.SYSTickMoteur1Duty){
		CMD.SYSTickMoteur1Duty = 0;
		WriteBoard(CMD.SMOTEUR,38,MOTEUR1);
		UpdateBoardLine(MOTEUR1);
		Sample(4);



		if(SegReturn() == 7){
			TBLPrint[0] = 'D';
			TBLPrint[1] = 'M';
			TBLPrint[2] = '1';
			TBLPrint[3] = 0;
			TBLPrint[4] = CMD.FREQDUTY[0];
			TBLPrint[5] = CMD.FREQDUTY[1];
			TBLPrint[6] = CMD.FREQDUTY[2];
			TBLPrint[7] = CMD.FREQDUTY[3];
		}

		if(CMD.LCDMoteur1){
			char TBMoteur[10] = {0};
			sprintf(TBMoteur,"Duty:%s",(CMD.SMOTEUR+1));
			WriteLCD(TBMoteur,12,0);
		}
	}

	if(CMD.SYSTickExtInt){
		CMD.SYSTickExtInt = 0;
		int i,j,rep;

		for(i=0;i<4;i++){
			for(j=0;j<4;j++){
				if(CMD.ClavMat[i][j]){
					rep = ((i<<2)+(j));
					CMD.ClavMat[i][j] = 0;
					break;
				}
			}
		}

		switch (rep){
			case 0x00 :
				if(CMD.LCDMoteur1){
					CMD.CommandValue = 0x8120;
				}
				else {
					CMD.CommandValue = 0x8110;
				}
				break;
			case 0x01 :
				if(CMD.Moteur&0x01){
					CMD.CommandValue = 0x2200;
				}
				else{
					CMD.CommandValue = 0x2100;
				}
				break;
			case 0x02:
				CMD.CommandValue = 0x2410;
				break;

			case 0x03 :
				CMD.CommandValue = 0x2420;
				break;

			case 0x04 :
				//RPM
				if(CMD.LCDMoteur1){
					CMD.CommandValue = 0x1310;
				}
				else{
					CMD.CommandValue = 0x1330;
				}
				break;

			case 0x05 :
				if(CMD.Moteur&0x02){
					CMD.CommandValue = 0x4200;
				}
				else{
					CMD.CommandValue = 0x4100;
				}
				break;

			case 0x06:
				CMD.CommandValue = 0x4410;
				break;

			case 0x7 :
				CMD.CommandValue = 0x4420;
				break;
			case 0x8 :
				//Duty
				if(CMD.LCDMoteur1){
					CMD.CommandValue = 0x1320;
				}
				else{
					CMD.CommandValue = 0x1340;
				}
				break;
			case 0x09:
				//RTC SEGMENT
				CMD.CommandValue = 0x1370;
				break;
			case 0x0A:
				if(CMD.LCDTemp1){
					CMD.LCDTemp1 = 0;
					CMD.LCDTemp2 = 1;
					if(CMD.LCDCelsius){
						CMD.CommandValue = 0x1350;
					}
					else{
						CMD.CommandValue = 0x1360;
					}
				}
				else{
					CMD.LCDTemp1 = 1;
					CMD.LCDTemp2 = 0;
					if(CMD.LCDCelsius){
						CMD.CommandValue = 0x1380;
					}
					else{
						CMD.CommandValue = 0x1390;
					}
				}

				break;
			case 0x0B:
				if(CMD.LCDCelsius){
					CMD.LCDCelsius = 0;
					CMD.LCDFareneith = 1;
				}
				else{
					CMD.LCDCelsius = 1;
					CMD.LCDFareneith = 0;
				}
				if(CMD.LCDTemp1){
					if(CMD.LCDCelsius){
						CMD.CommandValue = 0X1380;
					}
					else{
						CMD.CommandValue = 0X1390;
					}
				}
				else{
					if(CMD.LCDCelsius){
						CMD.CommandValue = 0X1350;
					}
					else{
						CMD.CommandValue = 0X1360;
					}
				}


				break;
			case 0x0C:
				break;
			case 0x0D:
				break;
			case 0x0E:
				break;
			case 0x0F:
				break;
			default :
				break;

		}

		Execute();

		IO0IntEnF = 0x0000000F;
		IO0IntClr = 0x0f;
	}

}

//interrupts des GPIO
void EINT3_IRQHandler(void){
	int WhoInt,i;


	if(IO0IntStatF){
		WhoInt = IO0IntStatF;
		IO0IntClr = 0x0f;
		IO0IntEnF = 0x00000000;

		switch (WhoInt){
			case 1 :
				WhoInt = 0;
				FIO0PIN0 = 0x10;
				for(i=0;i<40;i++){}
				if((FIO0PIN0&0x1)){
					CMD.ClavMat[WhoInt][0] = 1;
					//LLI("1x1");
					FIO0PIN0 = 0x00;
					break;
				}
				else{
					FIO0PIN0 = 0x20;
					for(i=0;i<40;i++){}
					if((FIO0PIN0&0x1)){
						CMD.ClavMat[WhoInt][1] = 1;
						//LLI("1x2");
						FIO0PIN0 = 0x00;
						break;
					}
					else{
						FIO0PIN0 = 0x40;
						for(i=0;i<40;i++){}
						if((FIO0PIN0&0x1)){
							CMD.ClavMat[WhoInt][2] = 1;
							//LLI("1x3");
							FIO0PIN0 = 0x00;
							break;
						}
						else{
							CMD.ClavMat[WhoInt][3] = 1;
							//LLI("1x4");
							FIO0PIN0 = 0x00;
							break;
						}
					}
				}
				break;
			case 2 :
				WhoInt = 1;
				FIO0PIN0 = 0x10;
				for(i=0;i<40;i++){}
				if((FIO0PIN0&0x2)){
					CMD.ClavMat[WhoInt][0] = 1;
					//LLI("2x1");
					FIO0PIN0 = 0x00;
					break;
				}
				else{
					FIO0PIN0 = 0x20;
					for(i=0;i<40;i++){}
					if((FIO0PIN0&0x2)){
						CMD.ClavMat[WhoInt][1] = 1;
						//LLI("2x2");
						FIO0PIN0 = 0x00;
						break;
					}
					else{
						FIO0PIN0 = 0x40;
						for(i=0;i<40;i++){}
						if((FIO0PIN0&0x2)){
							CMD.ClavMat[WhoInt][2]= 1;
							//LLI("2x3");
							FIO0PIN0 = 0x00;
							break;
						}
						else{
							CMD.ClavMat[WhoInt][3] = 1;
							//LLI("2x4");
							FIO0PIN0 = 0x00;
							break;
						}
					}
				}
				break;
			case 4 :
				WhoInt = 2;
				FIO0PIN0 = 0x10;
				for(i=0;i<40;i++){}
				if((FIO0PIN0&0x4)){
					CMD.ClavMat[WhoInt][0] = 1;
					//LLI("3x1");
					FIO0PIN0 = 0x00;
					break;
				}
				else{
					FIO0PIN0 = 0x20;
					for(i=0;i<40;i++){}
					if((FIO0PIN0&0x4)){
						CMD.ClavMat[WhoInt][1] = 1;
						//LLI("3x2");
						FIO0PIN0 = 0x00;
						break;
					}
					else{
						FIO0PIN0 = 0x40;
						for(i=0;i<40;i++){}
						if((FIO0PIN0&0x4)){
							CMD.ClavMat[WhoInt][2] = 1;
							//LLI("3x3");
							FIO0PIN0 = 0x00;
							break;
						}
						else{
							CMD.ClavMat[WhoInt][3]= 1;
							//LLI("3x4");
							FIO0PIN0 = 0x00;
							break;
						}
					}
				}
				break;
			case 8 :
				WhoInt = 3;
				FIO0PIN0 = 0x10;
				for(i=0;i<40;i++){}
				if((FIO0PIN0&0x8)){
					CMD.ClavMat[WhoInt][0] = 1;
					//LLI("4x1");
					FIO0PIN0 = 0x00;
					break;
				}
				else{
					FIO0PIN0 = 0x20;
					for(i=0;i<40;i++){}
					if((FIO0PIN0&0x8)){
						CMD.ClavMat[WhoInt][1] = 1;
						//LLI("4x2");
						FIO0PIN0 = 0x00;
						break;
					}
					else{
						FIO0PIN0 = 0x40;
						for(i=0;i<40;i++){}
						if((FIO0PIN0&0x8)){
							CMD.ClavMat[WhoInt][2] = 1;
							//LLI("4x3");
							FIO0PIN0 = 0x00;
							break;
						}
						else{
							CMD.ClavMat[WhoInt][3] = 1;
							//LLI("4x4");
							FIO0PIN0 = 0x00;
							break;
						}
					}
				}
				break;
			default :
				break;

		}
		CMD.SYSTickExtInt = 1;
		IO0IntClr = 0x0f;
	}

	//IO0IntEnF = 0x0000000F;

}

/*
void I2C2_IRQHandler(void){

	if(I2C2STAT == 0x08){
		I2C2DAT = CMD.IICSlaveAdd;
	}
	else{
		//I2C2DAT = 0x55;

	}



	CMD.IICFlag = 1;
	I2C2CONCLR |= 0x8;
}
*/







