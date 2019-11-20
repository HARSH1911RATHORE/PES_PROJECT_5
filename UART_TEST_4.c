
/**
 * @file    UART_TEST.c
 * @brief   Application entry point.
 */

#include <MKL25Z4.H>

#include <stdio.h>

#include "gpio_defs.h"

#include "UART.h"                    //all the library functions defined for the uart and circular buffer functions
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include<stdlib.h>
#include "uCUnit.h"
#include "logger.h"
#include "delay.h"
#include "circular_buff.h"

volatile static char Application_Count[256];            //maintains the application count
volatile static int mode=0;								//mode initilaised to zero

volatile static int application_Mode = 1;               //polling or echo mode

volatile static uint16_t n=0;
volatile static uint16_t x=0;							//all the variables needed to calculate timer value
volatile static uint16_t mili_value=0;
volatile static uint16_t second_value=0;
volatile static uint16_t seconds=0;
volatile static uint16_t minutes=0;
volatile static uint16_t min[100];
volatile static uint16_t sec[100];
volatile static uint16_t mil[100];
volatile static char Appln_Count[256];


void Init_SysTick(void){                                  //systick timer set to 10 hz
	SysTick->LOAD = (48000000L/160*2);
	NVIC_SetPriority(SysTick_IRQn,3);
	SysTick->VAL=0;
	SysTick->CTRL=SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;

}

void SysTick_Handler()                                  // value of systick timer count maintained in n
{
	n++;
}

void timer()
{
	x=n*20;
	mili_value=x%100;
	second_value=x/100;
	seconds=second_value%60;                           //printing the timer value form the start of program
	minutes=second_value/60;
	Send_String_Poll("\n\r");
	Send_String_Poll("0:");
	sprintf((char *) min, "%d", minutes);            //converting integer to integer string
	Send_String_Poll(min);
	Send_String_Poll(":");
	sprintf((char *) sec, "%d",seconds );
	Send_String_Poll(sec);
	Send_String_Poll(":");
	sprintf((char *) mil, "%d",mili_value );
	Send_String_Poll(mil);
	Send_String_Poll("\n\r");


}

int main (void) {

	int i;
	uint8_t buffer[80], c, * bp;					// definingt the vraiables in which value is stored
	Init_SysTick();                                  //calling the systick timer
	timer();
	Control_RGB_LEDs(0, 0, 1);						//blu led at start of function
	Delay (2000);
#ifdef test_mode
	mode = 0;                                     //flags set during various test modes

#endif

#ifdef debug_mode
	mode = 1;									//flags set during various test modes

#endif

#ifdef status_mode
	mode = 2;									//flags set during various test modes

#endif

	for (i=0;i<=sizeof(Application_Count);i++)
	{
		Application_Count[i]=0;               //initialising count value to 0
	}

	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	Init_UART0(115200);                      //setting the baud rate

	Init_RGB_LEDs();						//setting the leds ,initializing them

	logger(uart_init,status,true);
	timer();
	Delay (100);



	if (mode==0)
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
		UCUNIT_CheckIsNotNull(TB.Data);

		UCUNIT_TestcaseEnd();
		UCUNIT_WriteSummary();
	}



#if USE_UART_INTERRUPTS==0             // Polling version of code
	logger(uart_poll,status,true);     //calling log message for polling
	timer();
	Delay (100);						//setting delay between different logging states
	Send_String_Poll("\n\rHello, World!\n\r");


	if(application_Mode==1)			  //in application mode printing log message for the mode
	{
		logger(uart_application,status,true);
		timer();
		Delay (100);
	}
	else							//in echo mode printing log message for the mode
	{
		logger(uart_echo,status,true);
		timer();
		Delay (100);
	}
	Control_RGB_LEDs(0, 0, 1);	   //led green for receive during echo
	Delay (1000);
	while (1) {

		c = UART0_Receive_Poll();
		Application_Count[c]=Application_Count[c]+1;    //maintaining count of characters in application_count
		if (application_Mode==1)
		{

			if (c=='=')
			{
				Send_String_Poll("\n\r-------------------------The Report of the Charecters is -------------------\n");
				for (i=0;i<=sizeof(Application_Count);i++)
				{
					if (Application_Count[i]>0)
					{
						UART0_Transmit_Poll(i);                   //printing the characters one by one
						UART0_Transmit_Poll('-');						//printing the count value in integer form
						UART0_Transmit_Poll(Application_Count[i]+48);
						Send_String_Poll("\n\r");

					}
				}

				Send_String_Poll("\n-------------------------The Report Ends Here  -------------------\n");

			}
		}

		UART0_Transmit_Poll(c);                           //transmission of character when it is ready

		timer();
	}
	Control_RGB_LEDs(0, 1, 0);
	Delay (10000);                                       //switch to green led

#elif USE_UART_INTERRUPTS==1 							// Interrupt version of code
	logger(uart_interrupt,status,true);
	timer();
	Delay (100);
	Send_String("\n\rHello, World!\n\r");

	char add[10]="test";
	volatile int remove;
	logger(buffer_init,status,true);					//buffer initialization done
	timer();

	int valid=Circular_Buffer_Valid((&TxQ));
	logger(buffer_valid,status,true);                  //check if buffer is valid by checking it to null
	if (mode==1)
	{
		if (valid==0)
		{
			logger(buffer_valid,debug,false);        //printing that it is invalid if equal to null
			timer();
			Control_RGB_LEDs(1, 0, 0);              //glowing red led in that case
			Delay (1000);
		}
		else
		{
			logger(buffer_valid,debug,true);        //printing valid buffer
			timer();
		}
	}

	remove=Circular_Buffer_Remove(&RxQ);         //remove elements from buffer
	logger(buffer_remove,status,true);
	if (mode==1)
	{											//in debug mode
		if (Circular_Buffer_Size(&RxQ) == 0)   //check if size at start is 0, if yes error while removing elements
		{
			logger(buffer_remove,debug,false);
			timer();
			Control_RGB_LEDs(1, 0, 0);        //red led for error
			Delay (1000);
		}
		else
		{
			logger(buffer_remove,debug,true);
			timer();
			Control_RGB_LEDs(0, 0, 1);       //blue led for pass
			Delay (1000);
		}
	}

	Circular_Buffer_Add(&TxQ, *add);         //add elements to buffer
	logger(buffer_add,status,true);
	if (mode==1)							//in debug mode
	{
		if (Circular_Buffer_Full(&TxQ))    //if buffer full error in adding elements
		{
			logger(buffer_add,debug,false);
			timer();
			Control_RGB_LEDs(1, 0, 0);		//red led glows
			Delay (1000);
		}
		else
		{
			logger(buffer_add,debug,true);    //else no error and green led glows
			timer();
			Control_RGB_LEDs(0, 1, 0);
			Delay (1000);
		}
	}
	logger(uart_echo,status,true);          //echo mode
	timer();


	while (1) {


		while (Circular_Buffer_Size(&RxQ) == 0)              // Blocking receive

			; // wait for character to arrive

		c = Circular_Buffer_Remove(&RxQ);

		// Blocking transmit

		sprintf((char *) buffer, "You pressed %c\n\r", c);
		timer();

		bp = buffer;										 // enqueue string

		while (*bp != '\0') {

			// copy characters up to null terminator

			while (Circular_Buffer_Full(&TxQ))

				; 														// wait for space to open up

			Circular_Buffer_Add(&TxQ, *bp);

			bp++;

		}
		// start transmitter if it isn't already running

		if (!(UART0->C2 & UART0_C2_TIE_MASK)) {

			UART0->C2 |= UART0_C2_TIE(1);

		}

	}

#endif

	if (mode == 0)
	{
		UCUNIT_Shutdown();                //unit test
	}

}


