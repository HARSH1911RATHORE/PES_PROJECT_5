#ifndef UART_H

#define UART_H



#include <circular_buff.h>
#include <stdint.h>

#include <MKL25Z4.H>




#define USE_UART_INTERRUPTS 	(0) // 0 for polled UART communications, 1 for interrupt-driven

#define UART_OVERSAMPLE_RATE 	(16)			//oversampling ratio

#define BUS_CLOCK 			(24e6)				//bus clock value

#define SYS_CLOCK			(48e6)          //systick clock value



void Init_UART0(uint32_t baud_rate);

void UART0_Transmit_Poll(uint8_t data);                 //transmit poll and receive poll

uint8_t UART0_Receive_Poll(void);



void Send_String_Poll(uint8_t * str);
                                               //send characters one by one till array not equal to null
void Send_String(uint8_t * str);


uint32_t Rx_Chars_Available(void);

uint8_t	Get_Rx_Char(void);


extern Circilar_Buffer TxQ, RxQ;               //extern to access in different files Q_T TxQ, RxQ



#endif
