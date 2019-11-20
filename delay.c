#include <MKL25Z4.H>



void Delay (uint32_t dly) {

  volatile uint32_t t;                  //delay set to give time between logged messages and led blinking



	for (t=dly*10000; t>0; t--)

		;

}
