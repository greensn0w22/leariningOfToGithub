/*
 * ADC_DAC.h
 *
 *  Created on: 26 oct. 2017
 *      Author: mauclair
 */

#ifndef ADC_DAC_H_
#define ADC_DAC_H_

#include "stdint.h"

/* ADC */

static uint32_t volatile * const PtrADCR = (uint32_t *) 0x40034000;
static uint32_t volatile * const PtrADGDR = (uint32_t *) 0x40034004;
static uint32_t volatile * const PtrADSTAT = (uint32_t *) 0x40034030;
static uint32_t volatile * const PtrADINTEN = (uint32_t *) 0x4003400C;
static uint32_t volatile * const PtrAD0DR0 = (uint32_t *) 0x40034010;
static uint32_t volatile * const PtrAD0DR1 = (uint32_t *) 0x40034014;
static uint32_t volatile * const PtrAD0DR2 = (uint32_t *) 0x40034018;
static uint32_t volatile * const PtrAD0DR3 = (uint32_t *) 0x4003401C;
static uint32_t volatile * const PtrAD0DR4 = (uint32_t *) 0x40034020;
static uint32_t volatile * const PtrAD0DR5 = (uint32_t *) 0x40034024;
static uint32_t volatile * const PtrAD0DR6 = (uint32_t *) 0x40034028;
static uint32_t volatile * const PtrAD0DR7 = (uint32_t *) 0x4003402C;
static uint32_t volatile * const PtrADTRIM = (uint32_t *) 0x40034034;
#define AD0CR *PtrADCR
#define AD0GDR *PtrADGDR
#define ADSTAT *PtrADSTAT
#define AD0INTEN *PtrADINTEN
#define AD0DR0 *PtrAD0DR0
#define AD0DR1 *PtrAD0DR1
#define AD0DR2 *PtrAD0DR2
#define AD0DR3 *PtrAD0DR3
#define AD0DR4 *PtrAD0DR4
#define AD0DR5 *PtrAD0DR5
#define AD0DTRIM *PtrADTRIM

/* DAC */

static uint32_t volatile * const PtrDACR = (uint32_t *) 0x4008C000;
static uint32_t volatile * const PtrDACCTRL = (uint32_t *) 0x4008C004;
static uint32_t volatile * const PtrDACCNTVAL = (uint32_t *) 0x4008C008;
#define DACR *PtrDACR
#define DACCTRL *PtrDACCTRL
#define DACCNTVAL *PtrDACCNTVAL

#endif /* ADC_DAC_H_ */
