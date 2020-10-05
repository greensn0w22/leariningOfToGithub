/*
 * PID.cpp
 *
 *  Created on: 5 mars 2020
 *      Author: Julien
 */
#include <stdint.h>
#include "PID.h"




PID_c::PID_c(	float iniKp,
				float iniKi,
				float iniKd,
				float mSratio,
				int mOutput
)
{
	PIDstruct.kp = 0;
	PIDstruct.ki = 0;
	PIDstruct.kd = 0;
	PIDstruct.objectiveTemp = 0;
	PIDstruct.maxOutput = mOutput;
	PIDstruct.previousInput = 0;
	PIDstruct.pwm = 0;

	PIDstruct.kp = iniKp;
	PIDstruct.ki = iniKi*(mSratio/1000); /* Ratio on Sec */
	PIDstruct.kd = iniKd/(mSratio/1000); /* Ratio on Sec */

	PIDstruct.objectiveTemp = 0;
}

int PID_c::compute(
		//uint32_t newTime,
		unsigned char input
)
{
	static float sum = 0;
	float output = 0;

	float error = (PIDstruct.objectiveTemp - input);
	float diffInput = (input - PIDstruct.previousInput);

	//float PID_p = PIDstruct.kp * error; /* Proportional answer*/
	float PID_p = PIDstruct.kp * diffInput;
	float PID_i = PIDstruct.ki * error; /* Integral answer */
	float PID_d = PIDstruct.kd * diffInput;/* Derivative answer */

	sum += PID_i;
	sum -= PID_p;
	if(sum > PIDstruct.maxOutput) sum = PIDstruct.maxOutput;
	else if(sum < 1) sum = 1;

	//output = PID_p;
	output += sum - PID_d;

	if(output > PIDstruct.maxOutput) output = PIDstruct.maxOutput;
	else if(output < 1) output = 1;

	PIDstruct.previousInput = input;
	PIDstruct.pwm = (int) output;
	return PIDstruct.pwm;
}

void PID_c::setObjective
(
		unsigned char newObj
)
{
	PIDstruct.objectiveTemp = newObj;
}

PID_c::~PID_c()
{

}

PID_c::PID_c()
{

}
