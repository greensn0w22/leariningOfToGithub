/*
 * PID.h
 *
 *  Created on: 5 mars 2020
 *      Author: Julien
 */

#ifndef PID_H_
#define PID_H_

#include <stdint.h>

struct PID_t{
	float kp; /* Proportional */
	float ki; /* Integral */
	float kd; /* Derivative */
	unsigned char   objectiveTemp; /* Temperature you wish 0 - 255 */
	int  			previousInput;
	int 			maxOutput;
	int				pwm; /* output PWM value */
};

class PID_c{

public : /* Variable  */

private: /* Variable  */
	PID_t PIDstruct;

public : /* Functions */
	PID_c(	float iniKp,
			float iniKi,
			float iniKd,
			float msratio,
			int mOutput);

	int	compute(/*uint32_t newTime,*/unsigned char input);

	void setObjective(unsigned char newObj);


	~PID_c();
private: /* Functions */
	PID_c();

};



#endif /* PID_H_ */
