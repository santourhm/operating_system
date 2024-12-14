#include "uart.h"
#include "uart_reg.h"
#include "plic.h"
#include "console.h"
#include "cep_platform.h"
// Tampon pour stocker le caractère reçu par interruption
volatile char rx_char = 0;
volatile int char_received = 0;
// Control bits
/* #define UART_TXEN    0x1
#define UART_RXEN    0x1 */
#define UART_RXIE    0x2



// Initialisation de l'UART
void uart_init(void) {
    // Désactiver toutes les interruptions UART
    //uart_write_reg(UART_IE, 0);

    // Configurer le débit en bauds (115200)
    uart_write_reg(UART_DIV, 138);

    // Activer TX et RX
    uart_write_reg(UART_TXCTRL, UART_TXEN);
    uart_write_reg(UART_RXCTRL, UART_RXEN);

    // Activer l'interruption RX
    uart_write_reg(UART_IE, UART_RXIE);

    // Configurer le PLIC pour gérer les interruptions UART
    plic_init();
    setup_interrupts() ;
}
void uart_interrupt_handler(void){
    uint32_t rx_data = uart_read_reg(UART_RXDATA);

    // Vérifier si des données sont disponibles
    if (!(rx_data & RXDATA_EMPTY)) {
        rx_char = (char)(rx_data & RXDATA_MASK);
        traite_car(rx_char);
        char_received = 1;  // Indiquer qu'un caractère a été reçu
    }
    //plic_init();
    //setup_interrupts() ;
}

void setup_interrupts(void) {
    // Enable external interrupts in mie
    uint32_t mie;
    __asm__ volatile("csrr %0, mie" : "=r"(mie));
    mie |= (1 << 11);  // External interrupt
    __asm__ volatile("csrw mie, %0" : : "r"(mie));
    
    // Enable global interrupts in mstatus
    uint32_t mstatus;
    __asm__ volatile("csrr %0, mstatus" : "=r"(mstatus));
    mstatus |= (1 << 3);  // Global interrupt enable (MIE bit)
    __asm__ volatile("csrw mstatus, %0" : : "r"(mstatus));
    __asm__("csrw mtvec, %0" : : "r"(uart_interrupt_handler));
    }




int32_t uart_char_available(){
    return char_received;
}
void uart_puts(const char *str) {
    while (*str != '\0') {
        uart_putchar(*str);
        str++;
    }
}





char uart_getchar(void){
    return (char)(rx_char & 0xFF);
}

int uart_is_available(){
    return char_received;
}


void uart_putchar(char c) {
    uint32_t tx_data;
    
    // Wait until TX FIFO is not full
    do {
        tx_data = uart_read_reg(UART_TXDATA);
        
    } while (tx_data & 0x80000000);
    
    // Write the character to transmit
    uart_write_reg(UART_TXDATA, c);
}