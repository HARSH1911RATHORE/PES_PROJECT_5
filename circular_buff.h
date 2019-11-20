#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H
#include <stdint.h>
#include <MKL25Z4.h>

#define Q_MAX_SIZE (256)       //Define the Size of The circular buffer here

#define critical_section 1     //define critical section

typedef struct {

  unsigned int Head; // Index of the element to be output from the buffer -the Oldest Element
  unsigned int Tail; // Index of next element to be input in the circular buffer
  unsigned int Size; // Number of elements in use
  char* Data;        // Pointer to the Circular Buffer
  unsigned int capacity;    //Stores the size of the buffer
  unsigned int full;        //Flag that stores if the buffer is full

} volatile Circilar_Buffer;



typedef enum {	Sucess,                      //No error
				Buff_full,					 //Buffer is full, Item cannot be added
				Buff_empty,					 //Buffer is Empty , No item cannot be removed
				Buff_Not_Initialized,        //Buffer is not Initialized
				Buff_not_Valic,				 //Buffer is not valid
				undefined					 //undefined Error
			}error_enum;



extern void Circular_Buffer_Print(Circilar_Buffer * CB);
extern int Circular_Buffer_Empty(Circilar_Buffer * CB);               //prototype declaration
extern int Circular_Buffer_Full(Circilar_Buffer * CB);
extern int Circular_Buffer_Size(Circilar_Buffer * CB);					//define as extern so that it can be used in other files
extern error_enum Circular_Buffer_Add(Circilar_Buffer * CB, uint8_t d);
extern uint8_t Circular_Buffer_Remove(Circilar_Buffer * CB);
extern void Circular_Buffer_Init(Circilar_Buffer * CB);
extern void Circular_Buffer_free(Circilar_Buffer * CB);
extern int Circular_Buffer_intialized(Circilar_Buffer * CB);
extern int Circular_Buffer_Valid(Circilar_Buffer * CB);

#endif // QUEUE_H
