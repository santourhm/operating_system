#ifndef PLIC_H
#define PLIC_H

#include <inttypes.h>
#include "uart.h"
// PLIC Memory Map
#define PLIC_BASE           0x0C000000
#define PLIC_ENABLE         (PLIC_BASE + 0x2000)
#define PLIC_SOURCE         (PLIC_BASE + 0x0)
#define PLIC_TARGET         (PLIC_BASE + 0x200000)

// PLIC Configuration
#define UART_IRQ            1
#define UART_PRIORITY       3

void plic_init(void);


#endif // PLIC_H