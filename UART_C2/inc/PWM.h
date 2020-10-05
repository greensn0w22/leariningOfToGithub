/*
 * PWM.h
 *
 *  Created on: 12 oct. 2017
 *      Author: mauclair
 */

#ifndef PWM_H_
#define PWM_H_

#include "stdint.h"

/* Registres du module PWM1 */
static uint32_t volatile * const PtrPWM1IR = (uint32_t *) 0x40018000;
static uint32_t volatile * const PtrPWM1TCR = (uint32_t *) 0x40018004;
static uint32_t volatile * const PtrPWM1TC = (uint32_t *) 0x40018008;
static uint32_t volatile * const PtrPWM1PR = (uint32_t *) 0x4001800C;
static uint32_t volatile * const PtrPWM1PC = (uint32_t *) 0x40018010;
static uint32_t volatile * const PtrPWM1MCR = (uint32_t *) 0x40018014;
static uint32_t volatile * const PtrPWM1MR0 = (uint32_t *) 0x40018018;
static uint32_t volatile * const PtrPWM1MR1 = (uint32_t *) 0x4001801C;
static uint32_t volatile * const PtrPWM1MR2 = (uint32_t *) 0x40018020;
static uint32_t volatile * const PtrPWM1MR3 = (uint32_t *) 0x40018024;
static uint32_t volatile * const PtrPWM1CCR = (uint32_t *) 0x40018028;
static uint32_t volatile * const PtrPWM1CR0 = (uint32_t *) 0x4001802C;
static uint32_t volatile * const PtrPWM1CR1 = (uint32_t *) 0x40018030;
static uint32_t volatile * const PtrPWM1CR2 = (uint32_t *) 0x40018034;
static uint32_t volatile * const PtrPWM1CR3 = (uint32_t *) 0x40018038;
static uint32_t volatile * const PtrPWM1MR4 = (uint32_t *) 0x40018040;
static uint32_t volatile * const PtrPWM1MR5 = (uint32_t *) 0x40018044;
static uint32_t volatile * const PtrPWM1MR6 = (uint32_t *) 0x40018048;
static uint32_t volatile * const PtrPWM1PCR = (uint32_t *) 0x4001804C;
static uint32_t volatile * const PtrPWM1LER = (uint32_t *) 0x40018050;
static uint32_t volatile * const PtrPWM1CTCR = (uint32_t *) 0x40018070;
#define PWM1IR *PtrPWM1IR
#define PWM1TCR *PtrPWM1TCR
#define PWM1TC *PtrPWM1TC
#define PWM1PR *PtrPWM1PR
#define PWM1PC *PtrPWM1PC
#define PWM1MCR *PtrPWM1MCR
#define PWM1MR0 *PtrPWM1MR0
#define PWM1MR1 *PtrPWM1MR1
#define PWM1MR2 *PtrPWM1MR2
#define PWM1MR3 *PtrPWM1MR3
#define PWM1CCR *PtrPWM1CCR
#define PWM1CR0 *PtrPWM1CR0
#define PWM1CR1 *PtrPWM1CR1
#define PWM1CR2 *PtrPWM1CR2
#define PWM1CR3 *PtrPWM1CR3
#define PWM1MR4 *PtrPWM1MR4
#define PWM1MR5 *PtrPWM1MR5
#define PWM1MR6 *PtrPWM1MR6
#define PWM1PCR *PtrPWM1PCR
#define PWM1LER *PtrPWM1LER
#define PWM1CTCR *PtrPWM1CTCR

#endif /* PWM_H_ */
