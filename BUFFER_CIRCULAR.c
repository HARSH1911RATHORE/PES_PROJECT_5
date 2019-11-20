#include "BUFFER_CIRCULAR.h"
#include<stdlib.h>






//Function to initialize values and assert buffer pointer
void Circular_Buffer_Init(Circilar_Buffer * CB) {

  unsigned int i;           //Temporary Variable

  CB->Head = 0;             //Initialize the Head pointer to zero
  CB->Tail = 0;             //Initialize the Tail pointer to zero
  CB->Size = 0;             //Initialize the Size pointer to zero
  CB->Data = malloc(sizeof(char)*Q_MAX_SIZE);             //Dynamically allocate memory for the buffer
          for (i=0; i<Q_MAX_SIZE; i++)
          {
        	  CB->Data[i]='_';                            //Initialize all the buffer with the inital value of '_'for debugging
          }
          CB->capacity = Q_MAX_SIZE;                      //initialize the capacity to the max size
          CB->full = 0;									  //Initialize the full flag to zero

}






//Function to Free Memory from from the circular buffer
void Circular_Buffer_free(Circilar_Buffer * CB) {

  free(CB->Data);

          CB->capacity = Q_MAX_SIZE;
          CB->full = 0;
}





//Function to Check if a buffer element is valid
int Circular_Buffer_Valid(Circilar_Buffer * CB) {

   if (CB == NULL)
   {
	   return 0;   //Buffer is not valid
   }
   else
   {
	   return 1;   //Buffer is valid
   }
}

//Function to Check if Buffer is initialized
int Circular_Buffer_intialized(Circilar_Buffer * CB) {


   if (CB->Data[1] == '_')       //Check if the first element is inititalized to '_'
   {
	   return 1;       //The buffer is valid
   }
   else
   {
	   return 0;      //Buffer is not valid
   }
}





//Function to print the values in the Buffer
void Circular_Buffer_Print(Circilar_Buffer * CB)
{
	printf("\n \r_________The Elements of Buffer are______________________\n \r");
	int i;
	for (i=0; i<Q_MAX_SIZE; i++)
	          {
	        	  printf(" %c ",CB->Data[i]);
	          }
	printf("\n \r_______________________________\n \r");
}



//Function to Empty a Circular buffer Buffer
int Circular_Buffer_Empty(Circilar_Buffer * CB) {

  return CB->Size == 0;

}


//Function to check the full flag of the buffer
int Circular_Buffer_Full(Circilar_Buffer * CB) {

  return (CB->Size == CB->capacity);      //Check if the buffer size is equal to buffer capacity

}


//Function to Check current buffer Size
int Circular_Buffer_Size(Circilar_Buffer * CB) {

	return CB->Size;

}


//Function to add an element with data d at the tail of the Circular buffer
error_enum Circular_Buffer_Add(Circilar_Buffer * CB, uint8_t d) {

	uint32_t masking_state;

  if (!Circular_Buffer_Full(CB)) {         //Check if the buffer is not full

    CB->Data[CB->Tail++] = d;              // Add the data d to the next element of the tail and increment the tail Pointer

    CB->Tail = CB->Tail % CB->capacity;     //Use Modulus operator to set the tail pointer to begining ig it exceeds the capacity

		masking_state = __get_PRIMASK();

		if (Circular_Buffer_Full(CB))
		{
			CB->full = 1;                    //Set the Full flag if the buffer is full
		}



		__disable_irq();    //Disable Interuppt Routine macro to protect the atomic  Critical function

    CB->Size++;

		// restore  interrupt masking state

		__set_PRIMASK(masking_state);



    return Sucess; // success

  }

  if (Circular_Buffer_Full(CB))
  {
	  //check if the Buffer is full, If yes Then return Buffer full
	  return Buff_full;
  }

  else

    return undefined; // Unknown Error

}


//Function to remove the oldest element
uint8_t Circular_Buffer_Remove(Circilar_Buffer * CB) {

  uint32_t masking_state;
  uint8_t t=0;
  CB->full = 0;

  // Check if the Buffer is Full before removing

  if (!Circular_Buffer_Empty(CB)) {

    t = CB->Data[CB->Head];

    CB->Data[CB->Head++] = '_'; // empty unused entries for debugging

		CB->Head  = CB->Head % CB->capacity;


		masking_state = __get_PRIMASK();

		__disable_irq();

    CB->Size--;

		__set_PRIMASK(masking_state);

  }

  return t;

}

