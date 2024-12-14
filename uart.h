#ifndef _UART_H_
#define _UART_H_

#include <inttypes.h>
// UART Base address

#define UART_BASE 0x10013000


// UART Register offsets
#define UART_TXDATA  0x00
#define UART_RXDATA  0x04
#define UART_TXCTRL  0x08
#define UART_RXCTRL  0x0C
#define UART_IE      0x10
#define UART_IP      0x14
#define UART_DIV     0x18



// Status bits
#define TXDATA_FULL  0x80000000
#define RXDATA_EMPTY 0x80000000
#define RXDATA_MASK  0xFF


// Function declarations
void uart_init(void);
char uart_getchar(void);
void uart_putchar(char c);
void uart_puts(const char *str);
int32_t uart_char_available();


#endif // UART_H