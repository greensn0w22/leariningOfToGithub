/*
 * UART.h
 *
 *  Created on: 16 oct. 2017
 *      Author: mauclair
 */

#ifndef UART_H_
#define UART_H_


/* UART0*/
static unsigned long volatile * const PtrUART0_RBR = (unsigned long *) 0x4000C000;
static unsigned long volatile * const PtrUART0_THR = (unsigned long *) 0x4000C000;
static unsigned long volatile * const PtrUART0_DLL = (unsigned long *) 0x4000C000;
static unsigned long volatile * const PtrUART0_DLM = (unsigned long *) 0x4000C004;
static unsigned long volatile * const PtrUART0_IER = (unsigned long *) 0x4000C004;
static unsigned long volatile * const PtrUART0_IIR = (unsigned long *) 0x4000C008;
static unsigned long volatile * const PtrUART0_FCR = (unsigned long *) 0x4000C008;
static unsigned long volatile * const PtrUART0_LCR = (unsigned long *) 0x4000C00C;
static unsigned long volatile * const PtrUART0_LSR = (unsigned long *) 0x4000C014;
static unsigned long volatile * const PtrUART0_SCR = (unsigned long *) 0x4000C01C;
static unsigned long volatile * const PtrUART0_ACR = (unsigned long *) 0x4000C020;
static unsigned long volatile * const PtrUART0_ICR = (unsigned long *) 0x4000C024;
static unsigned long volatile * const PtrUART0_FDR = (unsigned long *) 0x4000C028;
static unsigned long volatile * const PtrUART0_TER = (unsigned long *) 0x4000C030;

#define U0RBR *PtrUART0_RBR
#define U0THR *PtrUART0_THR
#define U0DLL *PtrUART0_DLL
#define U0DLM *PtrUART0_DLM
#define U0IER *PtrUART0_IER
#define U0IIR *PtrUART0_IIR
#define U0FCR *PtrUART0_FCR
#define U0LCR *PtrUART0_LCR
#define U0LSR *PtrUART0_LSR
#define U0SCR *PtrUART0_SCR
#define U0ACR *PtrUART0_ACR
#define U0ICR *PtrUART0_ICR
#define U0FDR *PtrUART0_FDR
#define U0TER *PtrUART0_TER



/* UART2*/
static unsigned long volatile * const PtrUART2_RBR = (unsigned long *) 0x40098000;
static unsigned long volatile * const PtrUART2_THR = (unsigned long *) 0x40098000;
static unsigned long volatile * const PtrUART2_DLL = (unsigned long *) 0x40098000;
static unsigned long volatile * const PtrUART2_DLM = (unsigned long *) 0x40098004;
static unsigned long volatile * const PtrUART2_IER = (unsigned long *) 0x40098004;
static unsigned long volatile * const PtrUART2_IIR = (unsigned long *) 0x40098008;
static unsigned long volatile * const PtrUART2_FCR = (unsigned long *) 0x40098008;
static unsigned long volatile * const PtrUART2_LCR = (unsigned long *) 0x4009800C;
static unsigned long volatile * const PtrUART2_LSR = (unsigned long *) 0x40098014;
static unsigned long volatile * const PtrUART2_SCR = (unsigned long *) 0x4009801C;
static unsigned long volatile * const PtrUART2_ACR = (unsigned long *) 0x40098020;
static unsigned long volatile * const PtrUART2_ICR = (unsigned long *) 0x40098024;
static unsigned long volatile * const PtrUART2_FDR = (unsigned long *) 0x40098028;
static unsigned long volatile * const PtrUART2_TER = (unsigned long *) 0x40098030;

#define U2RBR *PtrUART2_RBR
#define U2THR *PtrUART2_THR
#define U2DLL *PtrUART2_DLL
#define U2DLM *PtrUART2_DLM
#define U2IER *PtrUART2_IER
#define U2IIR *PtrUART2_IIR
#define U2FCR *PtrUART2_FCR
#define U2LCR *PtrUART2_LCR
#define U2LSR *PtrUART2_LSR
#define U2SCR *PtrUART2_SCR
#define U2ACR *PtrUART2_ACR
#define U2ICR *PtrUART2_ICR
#define U2FDR *PtrUART2_FDR
#define U2TER *PtrUART2_TER



/* UART3*/
static unsigned long volatile * const PtrUART3_RBR = (unsigned long *) 0x4009C000;
static unsigned long volatile * const PtrUART3_THR = (unsigned long *) 0x4009C000;
static unsigned long volatile * const PtrUART3_DLL = (unsigned long *) 0x4009C000;
static unsigned long volatile * const PtrUART3_DLM = (unsigned long *) 0x4009C004;
static unsigned long volatile * const PtrUART3_IER = (unsigned long *) 0x4009C004;
static unsigned long volatile * const PtrUART3_IIR = (unsigned long *) 0x4009C008;
static unsigned long volatile * const PtrUART3_FCR = (unsigned long *) 0x4009C008;
static unsigned long volatile * const PtrUART3_LCR = (unsigned long *) 0x4009C00C;
static unsigned long volatile * const PtrUART3_LSR = (unsigned long *) 0x4009C014;
static unsigned long volatile * const PtrUART3_SCR = (unsigned long *) 0x4009C01C;
static unsigned long volatile * const PtrUART3_ACR = (unsigned long *) 0x4009C020;
static unsigned long volatile * const PtrUART3_ICR = (unsigned long *) 0x4009C024;
static unsigned long volatile * const PtrUART3_FDR = (unsigned long *) 0x4009C028;
static unsigned long volatile * const PtrUART3_TER = (unsigned long *) 0x4009C030;

#define U3RBR *PtrUART3_RBR
#define U3THR *PtrUART3_THR
#define U3DLL *PtrUART3_DLL
#define U3DLM *PtrUART3_DLM
#define U3IER *PtrUART3_IER
#define U3IIR *PtrUART3_IIR
#define U3FCR *PtrUART3_FCR
#define U3LCR *PtrUART3_LCR
#define U3LSR *PtrUART3_LSR
#define U3SCR *PtrUART3_SCR
#define U3ACR *PtrUART3_ACR
#define U3ICR *PtrUART3_ICR
#define U3FDR *PtrUART3_FDR
#define U3TER *PtrUART3_TER




/* UART1*/
static unsigned long volatile * const PtrUART1_RBR = (unsigned long *) 0x40010000;
static unsigned long volatile * const PtrUART1_THR = (unsigned long *) 0x40010000;
static unsigned long volatile * const PtrUART1_DLL = (unsigned long *) 0x40010000;
static unsigned long volatile * const PtrUART1_DLM = (unsigned long *) 0x40010004;
static unsigned long volatile * const PtrUART1_IER = (unsigned long *) 0x40010004;
static unsigned long volatile * const PtrUART1_IIR = (unsigned long *) 0x40010008;
static unsigned long volatile * const PtrUART1_FCR = (unsigned long *) 0x40010008;
static unsigned long volatile * const PtrUART1_LCR = (unsigned long *) 0x4001000C;
static unsigned long volatile * const PtrUART1_MCR = (unsigned long *) 0x40010010;
static unsigned long volatile * const PtrUART1_LSR = (unsigned long *) 0x40010014;
static unsigned long volatile * const PtrUART1_MSR = (unsigned long *) 0x40010018;
static unsigned long volatile * const PtrUART1_SCR = (unsigned long *) 0x4001001C;
static unsigned long volatile * const PtrUART1_ACR = (unsigned long *) 0x40010020;
static unsigned long volatile * const PtrUART1_ICR = (unsigned long *) 0x40010024;
static unsigned long volatile * const PtrUART1_FDR = (unsigned long *) 0x40010028;
static unsigned long volatile * const PtrUART1_TER = (unsigned long *) 0x40010030;
static unsigned long volatile * const PtrUART1_RS485CTRL = (unsigned long *) 0x4001004C;
static unsigned long volatile * const PtrUART1_ADRMATCH = (unsigned long *) 0x40010050;
static unsigned long volatile * const PtrUART1_RS485DLY = (unsigned long *) 0x40010054;


#define U1RBR *PtrUART1_RBR
#define U1THR *PtrUART1_THR
#define U1DLL *PtrUART1_DLL
#define U1DLM *PtrUART1_DLM
#define U1IER *PtrUART1_IER
#define U1IIR *PtrUART1_IIR
#define U1FCR *PtrUART1_FCR
#define U1LCR *PtrUART1_LCR
#define U1MCR *PtrUART1_MCR
#define U1LSR *PtrUART1_LSR
#define U1MSR *PtrUART1_MSR
#define U1SCR *PtrUART1_SCR
#define U1ACR *PtrUART1_ACR
#define U1ICR *PtrUART1_ICR
#define U1FDR *PtrUART1_FDR
#define U1TER *PtrUART1_TER
#define U1_RS485CTRL *PtrUART1_RS485CTRL
#define U1_ADRMATCH *PtrUART1_ADRMATCH
#define U1_RS485DLY *PtrUART1_RS485CDLY

#endif /* UART_H_ */