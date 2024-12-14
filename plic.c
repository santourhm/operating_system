#include "plic.h"

void plic_init(void) {
    // Enable UART IRQ (IRQ 1)
    volatile uint32_t* const enable = (volatile uint32_t*)PLIC_ENABLE;
    *enable = 1 << UART_IRQ;

    // Set UART interrupt priority
    volatile uint32_t* const priority = (volatile uint32_t*)(PLIC_SOURCE + 4 * UART_IRQ);
    *priority = UART_PRIORITY;

    // Set threshold to 0 (accept all priorities)
    volatile uint32_t* const threshold = (volatile uint32_t*)PLIC_TARGET;
    *threshold = 0;
}
