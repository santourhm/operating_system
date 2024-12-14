#ifndef _TIMER__
#define _TIMER__
#include "inttypes.h"
#include "console.h"


void print_timer(char * s );

void trap_handler_timer(uint64_t mcause , uint64_t mie , uint64_t mip ); 

void init_traitant_timer(void (*traitant)(void));

void enable_timer();

uint32_t nbr_secondes(void);



#endif