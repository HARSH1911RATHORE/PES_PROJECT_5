/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    UART_TEST.c
 * @brief   Application entry point.
 */

#include <MKL25Z4.H>

#include <stdio.h>

#include "gpio_defs.h"
#include "logger.h"
#include "UART.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include <time.h>
void timestamp()
{
	char buff[80];
   // time_t ltime; /* calendar time */
   // ltime=time(NULL); /* get current cal time */
   // sprintf((char *) buff, " %s\n\r", asctime( localtime(&ltime)) );
    time_t clk = time(NULL);
    sprintf((char *) buff, " %s\n\r", ctime(&clk));
    Send_String_Poll(buff);
}

void Init_SysTick(void){
	SysTick->LOAD = (4800000L/16);
	NVIC_SetPriority(SysTick_IRQn,3);
	SysTick->VAL=0;
	SysTick->CTRL=SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;

}

void SysTick_Handler()
{
static int n=0;
Control_RGB_LEDs(n&1,n&1,n&1);
n++;
}

/*----------------------------------------------------------------------------

  MAIN function



	Change definition of USE_UART_INTERRUPTS in UART.h to select polled or

	interrupt-driven communication.

	 *----------------------------------------------------------------------------*/

int main (void) {

	uint8_t buffer[80], c, * bp;

	    BOARD_InitBootPins();
	    BOARD_InitBootClocks();
	    BOARD_InitBootPeripherals();
	  	/* Init FSL debug console. */
	    BOARD_InitDebugConsole();

	Init_UART0(115200);

	Init_RGB_LEDs();

	Init_SysTick();

	timestamp();

 	printf("\n\rGood morning!\n\r");



#if USE_UART_INTERRUPTS==0 // Polling version of code
	enum function_type function=uart_poll;
	enum log_level level=test;
	logger(function,level,true);
	Send_String_Poll("\n\rHello, World!\n\r");



	// Code listing 8.9, p. 233

	while (1) {

		c = UART0_Receive_Poll();

		Control_RGB_LEDs(0, 0, 1);

		UART0_Transmit_Poll(c);

		Control_RGB_LEDs(0, 0, 0);

	}

#elif USE_UART_INTERRUPTS==1 // Interrupt version of code
	enum function_type function=uart_interrupt;
	enum log_level level=test;
	logger(function,level,true);
	Send_String("\n\rHello, World!\n\r");

	while (1) {

		// Blocking receive

		while (Q_Size(&RxQ) == 0)

			; // wait for character to arrive

		c = Q_Dequeue(&RxQ);



		// Blocking transmit

		sprintf((char *) buffer, "You pressed %c\n\r", c);

		// enqueue string

		bp = buffer;

		while (*bp != '\0') {

			// copy characters up to null terminator

			while (Q_Full(&TxQ))

				; // wait for space to open up

			Q_Enqueue(&TxQ, *bp);

			bp++;

		}

		// start transmitter if it isn't already running

		if (!(UART0->C2 & UART0_C2_TIE_MASK)) {

			UART0->C2 |= UART0_C2_TIE(1);

		}

	}

#endif

}





