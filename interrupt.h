#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <inttypes.h>

// Interrupt causes
#define CAUSE_MACHINE_TIMER    7
#define CAUSE_MACHINE_EXTERNAL 11

// Function to initialize the unified interrupt system
void init_interrupt_system(void);

// Main interrupt handler
void trap_handler(void);

#endif