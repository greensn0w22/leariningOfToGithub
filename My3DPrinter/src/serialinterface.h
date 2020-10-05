/*
 * serialinterface.h
 *
 *  Created on: 8 févr. 2020
 *      Author: Julien
 */

#ifndef SERIALINTERFACE_H_
#define SERIALINTERFACE_H_
#include <stdint.h>
#include <vector>
#include "UART2.h"



#define MAX_X	100
#define MAX_Y	50

//if less than 27 you cant put any data in
//the board
#define TBL_X	58
#define TBL_Y	15

#define S_UP	("\e[A")
#define S_DOWN 	("\e[B")
#define S_RIGHT ("\e[C")
#define S_LEFT 	("\e[D")

#define S_RED 		("\u001b[31m")
#define S_WHITE		("\u001b[37m")

#define S_BK_BLACK	("\u001b[40m")
#define S_BK_GREEN	("\u001b[42m")

struct line_t{
	char 		name[11];
	uint8_t 	pos;
	uint64_t 	data;
	char		unit[4];
	bool		progress;
	uint8_t		progressBarLenght;
	uint8_t		prog_min;
	uint8_t		prog_max;
	char		text[TBL_X];
};




class serialInt  {
	public: 	//variables


	private:	//variables
		char	tableSeparator;
		uint8_t	termX;
		uint8_t termY;
		//rxData_t *test;
		Uart_2 *U2;


		std::vector<line_t> line;

	public: 	//fonctions

		serialInt(char separator);

		void printRx(void);

		char getRx(void);

		~serialInt();

		bool createLine(const char *name,
						uint8_t position,
						uint64_t data ,
						const char *unit,
						bool progressBar = false,
						uint8_t prog_min = 0,
						uint8_t prog_max = 0);

		bool updateLine(uint8_t position,uint64_t newData);

		void printError(const char *Error);

	private:	//fonctions
		serialInt();
		void reWriteTable();

		void positionToLine(uint8_t line);





};



#endif /* SERIALINTERFACE_H_ */
