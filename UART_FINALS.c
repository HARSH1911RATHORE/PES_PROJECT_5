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
#include "UART_CODE.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include<stdlib.h>
#include "BUFFER_CIRCULAR.h"
#include "LED.h"
#include "logger.h"

////////////Change Here//////////////////////////////
#include "uCUnit.h"



/*----------------------------------------------------------------------------

  MAIN function



	Change definition of USE_UART_INTERRUPTS in UART.h to select polled or

	interrupt-driven communication.

	 *----------------------------------------------------------------------------*/

volatile static int Application_Count[256];      //An array to store the counts of the number of occurances in application mode
/////////////////////////////////////////////////////////////////////////////////////////////
volatile static int mode = 1;
volatile static int application_Mode = 0;

int main (void) {

	int i;
	int buffer[80], c, * bp;

#ifdef test_mode
	mode = 0;

#endif

#ifdef debug_mode
	mode = 1;

#endif

#ifdef status_mode
	mode = 2;

#endif


	for (i=0;i<=sizeof(Application_Count);i++)
	{
		Application_Count[i]=0;
	}

	    BOARD_InitBootPins();
	    BOARD_InitBootClocks();
	    BOARD_InitBootPeripherals();
	  	/* Init FSL debug console. */
	    BOARD_InitDebugConsole();

	Init_UART0(115200);

	Init_RGB_LEDs();
 	printf("\n\rGood morning!\n\r");


 //Run POST TEST if Test Mode is 1
if (mode == 1)
{
/////////////////////////////////////////////test_Code_Here/////////////////////////////////
	    UCUNIT_Init();
		UCUNIT_TestcaseBegin("Circular Buffer Test Suite");
		// test cases ...

		Circilar_Buffer TB;
		Circular_Buffer_Init(&TB);



 		//Test Cases for initialization and to check if all the Pointers and FLags Are at Appropriate spots

 		 UCUNIT_CheckIsEqual(TB.Head, 0);
 		 UCUNIT_CheckIsEqual(TB.Tail, 0);
 		 UCUNIT_CheckIsEqual(TB.Size, 0);


 		//Test Cases for Test Data ACCESS

 		 UCUNIT_CheckIsEqual(TB.Data[0],'_');
		 UCUNIT_CheckIsEqual(TB.Data[1],'_');
    	 UCUNIT_CheckIsEqual(TB.Data[2],'_');
    	 UCUNIT_CheckIsEqual(TB.Data[3],'_');

 		//Add 1 2 3 4 in the 4 size circular Buffer
 		Circular_Buffer_Add(&TB,1);
 		Circular_Buffer_Add(&TB,2);
		Circular_Buffer_Add(&TB,3);
		Circular_Buffer_Add(&TB,4);

		//Test cases to check if the values are correctly written
    	 UCUNIT_CheckIsEqual(TB.Data[0],1);
    	 UCUNIT_CheckIsEqual(TB.Data[1],2);
    	 UCUNIT_CheckIsEqual(TB.Data[2],3);
    	 UCUNIT_CheckIsEqual(TB.Data[3],4);

		//To check if it pops out of the right routine
    	 int temp = Circular_Buffer_Remove(&TB);
    	 UCUNIT_CheckIsEqual(temp,1);


    	 //ADD Wrap Around Test    Bufffer State 5 2 3 4
    	 Circular_Buffer_Add(&TB,5);
    	 UCUNIT_CheckIsEqual(TB.Data[0],5);

    	 //Remove 2 more elements from Buffer  to make 5 _ _ _
    	 temp = Circular_Buffer_Remove(&TB);
    	 temp = Circular_Buffer_Remove(&TB);
    	 temp = Circular_Buffer_Remove(&TB);

    	 //Check to see if the last popped element is 4
    	 UCUNIT_CheckIsEqual(temp,4);

    	 //Wrap Around Remove Test
    	 temp = Circular_Buffer_Remove(&TB);
    	 UCUNIT_CheckIsEqual(temp,5);

    	 //TO check if the circular buffer pointer is valid
    	 UCUNIT_CheckIsNotNull(TB.Data)

 		UCUNIT_TestcaseEnd();
 		UCUNIT_WriteSummary();
}
/////////////////////////////////////////////////////////////////////////////////////////




#if USE_UART_INTERRUPTS==0 // Polling version of code
enum function_type function=uart_poll;
enum log_level level=test;
logger(function,level,true);
	Send_String_Poll("\n\rHello, World!\n\r");



	// Code listing 8.9, p. 233

	while (1) {

		c = UART0_Receive_Poll();
		Application_Count[c]=Application_Count[c]+1;
		if (application_Mode==1)
		{
		if (c=='=')
		{
			Send_String_Poll("\n\r-------------------------The Report of the Charecters is -------------------\n");
			for (i=0;i<=sizeof(Application_Count);i++)
				{

				   	if (Application_Count[i]>0)
				   	{

				   		UART0_Transmit_Poll(i);
						UART0_Transmit_Poll('-');
						UART0_Transmit_Poll(Application_Count[i]+48);
						//Send_String_Poll(atoi(Application_Count[i]));
						Send_String_Poll("\n\r");

				   	}
				}

				Send_String_Poll("\n-------------------------The Report Ends Here  -------------------\n");

				}
		}

		UART0_Transmit_Poll(c);



	}

#elif USE_UART_INTERRUPTS==1 // Interrupt version of code
	enum function_type function=uart_interrupt;
	enum log_level level=test;
	logger(function,level,true);
	Send_String("\n\rHello, World!\n\r");



	// Code listing 8.10, p. 234

	while (1) {

		// Blocking receive

		while (Circular_Buffer_Size(&RxQ) == 0)

			; // wait for character to arrive

		c = Circular_Buffer_Remove(&RxQ);



		// Blocking transmit

		sprintf((char *) buffer, "You pressed %c\n\r", c);

		// enqueue string

		bp = buffer;

		while (*bp != '\0') {

			// copy characters up to null terminator

			while (Circular_Buffer_Full(&TxQ))

				; // wait for space to open up

			Circular_Buffer_Add(&TxQ, *bp);

			bp++;

		}

		// start transmitter if it isn't already running

		if (!(UART0->C2 & UART0_C2_TIE_MASK)) {

			UART0->C2 |= UART0_C2_TIE(1);

		}

	}

#endif



	////////////////////////////////////////////////////////////////////////////
	if (mode == 1)
	{
		UCUNIT_Shutdown();
	}


}


