/*
 * motorclass.h
 *
 *  Created on: 6 avr. 2020
 *      Author: Julien
 */

#ifndef MOTORCLASS_H_
#define MOTORCLASS_H_
#include <stdint.h>
#define BOARD_V1 //sleep xye -> 2.6 & sleepZZ -> 2.7

#ifndef BOARD_V1
#define BOARD_V2 //sleep xye -> 1.19 && sleepZ -> 1.20
#endif

#define CLOCKWISE		0
#define NON_CLOCKWISE 	1

enum motorLetters_e {
	_x = 0,
	_y,
	_z,
	_e
};


class motorsXYEZ{
	public:		//variable
	private:	//variable


	public : 	//function
	motorsXYEZ();
	~motorsXYEZ();
	void step(motorLetters_e motor);
	bool changeDir(motorLetters_e motor, bool dir);

	void sleepXYE(bool state);
	void sleepZ(bool state);
	void updateZeroesStates(motorLetters_e motor, bool state);

	private:	//function

};



#endif /* MOTORCLASS_H_ */
