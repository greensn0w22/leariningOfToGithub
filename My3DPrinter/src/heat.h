/*
 * heat.h
 *
 *  Created on: 25 févr. 2020
 *      Author: Julien
 */

#ifndef HEAT_H_
#define HEAT_H_

#include "PID.h"

/*
#define BLOCK_KI 0
#define BLOCK_KP 0
#define BLOCK_KD 0

#define BED_KI 0
#define BED_KP 0
#define BED_KD 0
*/

enum heatSource_e{
	block,
	bed
};



class heat_c{
/* public variables*/
public:

/* private variables*/
private:
	uint8_t blockObjective;
	uint8_t bedObjective;
	PID_c	*pidBlock;


public:
	heat_c();
	~heat_c();

	void newTemp(heatSource_e ,uint8_t objective);
	void computePID(heatSource_e, uint8_t input);
	uint8_t getHeat(heatSource_e );

};


#endif /* HEAT_H_ */
