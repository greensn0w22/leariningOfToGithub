/*
 * serialinterface.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Julien
 */



#include "serialinterface.h"
#include <vector>
#include "fonctionsLPC.h"
#include <string.h>







rxData_t test;

serialInt::serialInt(){}


serialInt::~serialInt(){
	delete U2;
}

serialInt::serialInt(char separator){
	uint8_t i;
	uint8_t mX = TBL_X;
	uint8_t mY = TBL_Y;

	U2 = new Uart_2(&test);

	tableSeparator = separator;



	U2->sendString(S_WHITE);
	for (i = 0;i <=MAX_Y; i++){
		U2->sendString(S_UP);
	}
		U2->sendString("\r");
	for(i=0; i<=MAX_Y; i++){
		for(uint8_t j=0;j<=MAX_X; j++ ){
			U2->sendString(" ");
		}
		U2->sendString("\n\r");
	}
	for (i = 0;i <=MAX_Y; i++){
		U2->sendString(S_UP);
	}

	for(uint8_t y = 0; y<mY; y++)
	{
		for(uint8_t x = 0; x<mX; x++)
		{
			if((y == 0) || (y == (mY-1)))
			{
				U2->sendTbl(&separator,1);

			}
			else if((x == 0) || (x == (mX-1)))
			{
				U2->sendTbl(&separator,1);

			}
			else
			{
				U2->sendString(" ");
			}

		}
		U2->sendString("\n\r");
	}

	for (i = 0;i <= mY; i++){
		U2->sendString(S_UP);
	}

	termX = 0;
	termY = 0;

}

void serialInt::printRx(void){
	char dataIn = 0;
	while(U2->dataReady() == true){
		dataIn = U2->GetRx();
		U2->sendTbl(&dataIn, 1);
	}
}

char serialInt::getRx(void){
	char dataIn = 0;
	if(U2->dataReady() == true)
		dataIn = U2->GetRx();
	else dataIn = false;

	return dataIn;
}

bool serialInt::createLine(
	const char 	*name,
	uint8_t 	position,
	uint64_t 	data,
	const char 	*unit,
	bool 		progressBar,
	uint8_t 	prog_min,
	uint8_t 	prog_max ){

	line_t newElement;
	char tblTemp[20];
	uint8_t i = 0, j, temp = 0;

	//used line (i):
	//board border 	(0 	-> 1)
	//name display 	(2 	-> 11)
	//number display(14	-> 21)
	//unit display	(22	-> 24)
	//pgBar	min_num	(27	-> 29)
	//pgBar [		(30)
	//pgBarFill		(31 -> (TBL_X - 7))
	//pgBar ]		(TBL_X - 6)
	//pgBar	min_num	((TBL_X - 5) -> (TBL_X - 3))
	//board border 	((TBL_X - 2) -> (TBL_X - 1))


	if(position >= TBL_Y){
		//todo print code erreur
		return false;
	}

	if((TBL_X >= 27) == false){
		//todo print code erreur
		return false;
	}


	newElement.pos = position;
	newElement.data = data;
	newElement.progress = progressBar;
	newElement.prog_max = prog_max;
	newElement.prog_min = prog_min;





	newElement.text[0] = tableSeparator;
	newElement.text[1] = ' ';
	i = 2;


	if( strlen(name) >= 10){
		j = 0;
		for(; i<=11; i++){
			newElement.text[i] = name[j];
			newElement.name[j] = name[j];
			j++;

		}
		newElement.name[j] = '\0';
		newElement.text[i] = ' ';
		i++;
		;
	}
	else{

		for(j = 0; j < strlen(name); j++){
			newElement.text[i+j] = name[j];
			newElement.name[j] 	 = name[j];
		}
		newElement.name[j] 	 = '\0';
		i += j;

		temp = 10 - strlen(name);
		temp += i; //temp au niveau de i + la longueur ' ' nécessaire a remplir
		for(; i <= temp; i++){
			newElement.text[i] = ' ';

		}

	}

	newElement.text[i] = ':';
	i++;

	convNumToString(data, tblTemp);
	if( strlen(tblTemp) >= 8){ //max 8 case alloué à l'affichage de donnée
		j=0;
		for(; i<=21; i++){
			newElement.text[i] = tblTemp[j];
			j++;
		}
	}
	else{
		temp = 8-strlen(tblTemp);
		temp += i;

		for(; i < temp; i++){
			newElement.text[i] = ' ';

		}
		for(j = 0; j < strlen(tblTemp); j++){
			newElement.text[i+j] = tblTemp[j];
		}
		i += j;

	}

	if(strlen(unit) >= 3){ //max 3 case alloué à l'affichage de l'unité de mesure
		j=0;
		for(; i<=24; i++){
			newElement.text[i] = unit[j];
			newElement.unit[j] = unit[j];
			j++;
		}
		newElement.unit[j] = '\0';
	}
	else{
		for(j = 0; j < strlen(unit); j++){
			newElement.text[i+j] = unit[j];
			newElement.unit[j]   = unit[j];
		}
		newElement.unit[j]   = '\0';
		i += j;

		temp = 3-strlen(unit);
		temp += i;

		for(; i < temp; i++){
			newElement.text[i] = ' ';
		}


	}

	for(; i<28 ; i++){
		newElement.text[i] = ' ';
	}


	//création de la barre de progrès (x = 28)
	//todo condition de test avant création


	if(progressBar == true){
		convNumToString(prog_min, tblTemp);

		newElement.progressBarLenght = (TBL_X - 38); //Longueur du tableau par rapport tableau
												   //et aux autres caractères utilisés


		if(strlen(tblTemp) >= 3){ //max 3 case alloué à l'affichage de la prog_min
			j=0;
			for(; i<=30; i++){
				newElement.text[i] = tblTemp[j];
				j++;
			}
		}
		else{

			temp = 3-strlen(tblTemp);
						temp += i;

			for(; i < temp; i++){
				newElement.text[i] = ' ';
			}

			for(j = 0; j < strlen(tblTemp); j++){
						newElement.text[i+j] = tblTemp[j];
			}
			i += j;
		}

		newElement.text[i] = '[';
		i = 32;

		temp = i + (newElement.progressBarLenght - 1);

		for(; i<= temp; i++){
			newElement.text[i] = ' ';
		}

		newElement.text[i] = ']';
		i++;

		convNumToString(prog_max, tblTemp);
		if(strlen(tblTemp) >= 3){ //max 3 case alloué à l'affichage de la prog_max
			j=0;
			for(; i<=(TBL_X - 3); i++){
				newElement.text[i] = tblTemp[j];
				j++;
			}
		}
		else{
			for(j = 0; j < strlen(tblTemp); j++){
						newElement.text[i+j] = tblTemp[j];
			}
			i += j;
			temp = 3-strlen(tblTemp);
			temp += i;
			for(; i < temp; i++){
				newElement.text[i] = ' ';
			}
		}
	}
	//end of progress bar

	//filling
	for(; i < (TBL_X-1); i++){
		newElement.text[i] = ' ';
	}

	newElement.text[i] = tableSeparator;

	i++;

	newElement.text[TBL_X] = 3;
	;
	///fin INIT

	positionToLine(newElement.pos);
	i = 0;
	while(newElement.text[i] != 3){
		U2->sendTbl((newElement.text+i) , 1);
		i++;
	}

	line.push_back(newElement);
	termY = position;
	updateLine(newElement.pos, data);
	return true;
}

bool serialInt::updateLine(uint8_t position,uint64_t newData){
	uint8_t size = line.size(), i, j, temp;
	uint32_t max,pgBarL, fill;

	bool found = 0;
	char tblTemp[10];

	for(i = 0; i < size; i++){ //check if the line asked exist
		if(line[i].pos == position){
			found = 1;
			break;
		}

	}

	if(found == false){
		return false;
	}

	line[i].data = newData; //update du data
	positionToLine(position);
	while(termX < 14)
	{
		U2->sendString(S_RIGHT);
		termX++;
	}

	while(termX <= 21){
		U2->sendString(" ");
		termX++;
	}

	while(termX > 15)
	{
		U2->sendString(S_LEFT);
		termX--;
	}
	U2->sendString(S_LEFT);

	convNumToString(newData, tblTemp);
	if(strlen(tblTemp) >= 8){
		j=0;
		for(; termX<=21; termX++){
			line[i].text[termX] = tblTemp[j];
			U2->sendTbl((tblTemp+j) ,1);
			j++;
		}
	}
	else{
		temp = (8-strlen(tblTemp));
		temp += termX;

		for(; termX < temp; termX++){
			line[i].text[termX] = ' ';
			U2->sendString(" ");
		}
		for(j = 0; j < strlen(tblTemp); j++){
			line[i].text[termX + j] = tblTemp[j];
			U2->sendTbl((tblTemp+j) ,1);
		}

	}
	termX = 21;

	if(line[i].progress == true){
		pgBarL = (line[i].progressBarLenght);
		pgBarL = (line[i].progressBarLenght*100);
		max = (line[i].prog_max)*100;

		fill = (newData * pgBarL)/max;

		while(termX != 31){
			U2->sendString(S_RIGHT);
			termX++;
		}

		U2->sendString(S_BK_GREEN);
		for(j = 0; j < fill; j++){
			U2->sendString(" ");
		}
		U2->sendString(S_BK_BLACK);
		for(; j < line[i].progressBarLenght; j++){
			U2->sendString(" ");
		}

	}

	U2->sendString("\r");
	termX = 0;

	return true;
}

void serialInt::reWriteTable(){
	uint8_t mX = TBL_X;
	uint8_t mY = TBL_Y;
	uint8_t i;

	char separator = tableSeparator;



	U2->sendString(S_WHITE);
	for (i = 0;i <=MAX_Y; i++){
		U2->sendString(S_UP);
	}
		U2->sendString("\r");
	for(i=0; i<=MAX_Y; i++){
		for(uint8_t j=0;j<=70; j++ ){
			U2->sendString(" ");
		}
		U2->sendString("\n\r");
	}
	for (i = 0;i <=MAX_Y; i++){
		U2->sendString(S_UP);
	}

	for(uint8_t y = 0; y<mY; y++)
	{
		for(uint8_t x = 0; x<mX; x++)
		{
			if((y == 0) || (y == (mY-1)))
			{
				U2->sendTbl(&separator,1);

			}
			else if((x == 0) || (x == (mX-1)))
			{
				U2->sendTbl(&separator,1);

			}
			else
			{
				U2->sendString(" ");

			}

		}
		U2->sendString("\n\r");
	}

	for (i = 0;i <= mY; i++){
		U2->sendString(S_UP);
	}

	termX = 0;
	termY = 0;

}

void serialInt::printError(const char *Error){

	uint8_t i = 0,j=0;

	while(termY < (TBL_Y)){
		U2->sendString("\n");
		U2->sendString("\r");
		termY++;
	}
	termX = 0;

	for(i = 0; i<=3; i++){

		for(j = 0; j<=MAX_X; j++){
			U2->sendString(" ");
		}
		U2->sendString("\n");
		U2->sendString("\r");
		termY++;
	}

	while(termY > (TBL_Y)){
		U2->sendString(S_UP);
		termY--;
	}


	//TODO compteur de lettre pour le na dépasser le tableau

	U2->sendString(Error);

	U2->sendString("\r");

}


void serialInt::positionToLine(uint8_t line){
	U2->sendString("\r");
	termX = 0;
	if(termY > line){
		for(; termY > line; termY--){
			U2->sendString(S_UP);
		}
	}
	else{
		for(; termY < line; termY++){
			U2->sendString(S_DOWN);
		}
	}
}

