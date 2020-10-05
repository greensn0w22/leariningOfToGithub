/*
 * fonctionsLPC.h
 *
 *  Created on: 11 févr. 2020
 *      Author: Julien
 */

#ifndef FONCTIONSLPC_H_
#define FONCTIONSLPC_H_
#include <stdint.h>

uint8_t testLenght(uint64_t);
void convNumToString(uint64_t number, char *tbl);
uint64_t power(uint32_t number, uint8_t pow);
uint32_t convStringToNum(char* pointer, int lenght);
uint32_t abs(int32_t info);

#endif /* FONCTIONSLPC_H_ */
