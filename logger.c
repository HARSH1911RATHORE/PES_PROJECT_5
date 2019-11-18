
#include "logger.h"




void logger(enum function_type functions,enum log_level level,uint8_t message)
{
	log_receiver_available(functions, level, message);

	log_receiver_char(functions,level);                       //logger messages for the various function calls

	log_transmitter_available(functions,level,message);

	log_transmitter_char(functions,message);

	log_uart_mode_int(functions);                         //used an enum to call the logger messages of different
														  //function calls in their debug mode,test mode and status mode
	log_uart_mode_poll(functions);

	log_uart_init(functions);

	log_uart_echo(functions);

	log_uart_application(functions);

	log_buffer_full(functions,message);

	log_buffer_empty(functions,message);

	log_buffer_valid(functions,message);

	log_buffer_init(functions,message);

	log_buffer_add(functions,level,message);

	log_buffer_remove(functions,level,message);

	log_buffer_destory(functions,message);

}

void log_receiver_available(enum function_type functions,enum log_level level,uint8_t message)
{
	if(functions==all || functions==receiver_available)
	{
	if (level==test)											//prints message by checking the receiver availability for
		Send_String_Poll("\n\rReceiver test mode\n\r");
																  //the required data
	else if(level==debug)
		Send_String_Poll("\n\rhey\n\r");


	}

}


void log_receiver_char(enum function_type functions,enum log_level level)
{
	if(functions==all || functions==receive_char)
	{															//prints message to print the message about the receiver ready
	if (level==test)											//ready to accept characters
		Send_String_Poll("\n\r\n\r");

	else if(level==debug)
		Send_String_Poll("\n\r\n\r");

	}

}

void log_transmitter_available(enum function_type functions,enum log_level level,uint8_t message)
{
	if(functions==all || functions==transmitter_available)
	{
	if (level==test)
		if(message=true)
			Send_String_Poll("\n\rTRANSMIT BUFFER READY TO ACCEPT DATA\n\r");
		else
			Send_String_Poll("\n\rTRANSMIT BUFFER NOT READY TO ACCEPT DATA\n\r");

	else if(level==debug)
		Send_String_Poll("\n\r\n\r");

	}

}

void log_transmitter_char(enum function_type functions,enum log_level level)
{
	if(functions==all || functions==transmitter_char)
	{
	if (level==test)
		Send_String_Poll("\n\r\n\r");

	else if(level==debug)
		Send_String_Poll("\n\r\n\r");

	}
}

void log_uart_mode_int(enum function_type functions)
{
	if(functions==all || functions==uart_interrupt)                  //prints message if uart is in interrupt mode

		Send_String_Poll("\n\rUART INTERRUPT MODE\n\r");

}

void log_uart_mode_poll(enum function_type functions)				//prints message if uart is in polling mode
{
	if(functions==all || functions==uart_poll)

		Send_String_Poll("\n\rUART POLLING MODE\n\r");
}

void log_uart_init(enum function_type functions)				  //prints message for uart initialization
{
	if(functions==all || functions==uart_init)

		Send_String_Poll("\n\rUART INITIALIZATION\n\r");
}

void log_uart_echo(enum function_type functions)				//prints message if echo mode is defined
{
	if(functions==all || functions==uart_echo)

		Send_String_Poll("\n\rUART ECHO MODE\n\r");
}

void log_uart_application(enum function_type functions)			//prints message if application mode is defined
{
	if(functions==all || functions==uart_application)
		Send_String_Poll("\n\rUART APPLICATION MODE\n\r");
}

void log_buffer_full(enum function_type functions,enum log_level level)
{
	if(functions==all || functions==buffer_full)				//prints buffer full message
	{
	if (level==test)											//prints messages in debug ,test and status mode
		Send_String_Poll("\n\rCHECK IF BUFFER IS FULL\n\r");
	else if(level==debug)
		Send_String_Poll("\n\rCHECK IF BUFFER IS FULL\n\r");
	else
		Send_String_Poll("\n\rCHECK IF BUFFER IS FULL\n\r");

	}
}
void log_buffer_empty(enum function_type functions,enum log_level level)
{
	if(functions==all || functions==buffer_empty)              //prints buffer empty messages
	{
	if (level==test)
		Send_String_Poll("\n\rCHECK IF BUFFER IS EMPTY\n\r");  //prints messages in debug,test and status mode

	else if(level==debug)
		Send_String_Poll("\n\rCHECK IF BUFFER IS EMPTY\n\r");
	else
		Send_String_Poll("\n\rCHECK IF BUFFER IS EMPTY\n\r");

	}
}

void log_buffer_valid(enum function_type functions,enum log_level level)
{
	if(functions==all || functions==buffer_valid)				//check for validity of buffer
	{
	if (level==test)											//prints messages in debug,test and status mode
		Send_String_Poll("\n\rCHECK IF BUFFER POINTER IS VALID\n\r");
	else if(level==debug)
		Send_String_Poll("\n\rCHECK IF BUFFER POINTER IS VALID\n\r");
	else
		Send_String_Poll("\n\rCHECK IF BUFFER POINTER IS VALID\n\r");

	}
}

void log_buffer_init(enum function_type functions,enum log_level level)
{
	if(functions==all || functions==buffer_init)               //check for buffer pointer correct initialisation
	{
	if (level==test)
		Send_String_Poll("\n\rINITIALIZE THE BUFFER\n\r");     //prints messages in debug,test and status mode

	else if(level==debug)
		Send_String_Poll("\n\rINITIALIZE THE BUFFER\n\r");
	else
		Send_String_Poll("\n\rINITIALIZE THE BUFFER\n\r");

	}
}

void log_buffer_add(enum function_type functions,enum log_level level,uint8_t message)
{
	if(functions==all || functions==buffer_add)              //print message to add elements to the buffer
	{
	if (level==test)
	{														//check if buffer is already full before adding elements
		Send_String_Poll("\n\rADD NEW BUFFER ELEMENT\n\r");  //prints messages in debug,test and status mode
		if (message==true)
			Send_String_Poll("\n\rERROR:BUFFER IS ALREADY FULL\n\r");
	}
	else if(level==debug)
	{
		Send_String_Poll("\n\rBUFFER R\n\r");

	}
	else
		Send_String_Poll("\n\rADD NEW BUFFER ELEMENT\n\r");
}

}

void log_buffer_remove(enum function_type functions,enum log_level level,uint8_t message)
{
	if(functions==all || functions==buffer_remove)                  //print message to remove elements to the buffer
	{
	if (level==test)
	{
		Send_String_Poll("\n\rREMOVE BUFFER OLDEST ELEMENT\n\r"); //check if buffer is already empty before adding elements
		if (message==true)
			Send_String_Poll("\n\rERROR:BUFFER IS ALREADY EMPTY\n\r"); //prints messages in debug,test and status mode
	}
	else if(level==debug)
	{
		Send_String_Poll("\n\rBUFFER R\n\r");

	}
	else
		Send_String_Poll("\n\rREMOVE BUFFER OLDEST ELEMENT\n\r");
}
}

void log_buffer_destory(enum function_type functions,enum log_level level)
{
	if(functions==all || functions==buffer_destroy)
	{                                                           //print message to remove elements to the buffer
	if (level==test)
		Send_String_Poll("\n\rDESTROY THE BUFFER\n\r");        //prints messages in debug,test and status mode

	else if(level==debug)
		Send_String_Poll("\n\rDESTROY THE BUFFER\n\r");
	else
		Send_String_Poll("\n\rDESTROY THE BUFFER\n\r");

	}
}

