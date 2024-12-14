#include "interrupt.h"
#include "uart_reg.h"
#include "uart.h"
#include "timer.h"
#include "plic.h"
extern void mon_traitant(void);

void init_interrupt_system(void) {
    // Set up the unified trap handler
    __asm__("csrw mtvec, %0" : : "r"(trap_handler));
    
    // Enable both timer and external interrupts in mie
    uint32_t mie;
    __asm__ volatile("csrr %0, mie" : "=r"(mie));
    mie |= (1 << 7);   // Timer interrupt enable
    mie |= (1 << 11);  // External interrupt enable
    __asm__ volatile("csrw mie, %0" : : "r"(mie));
    
    // Enable global interrupts
    uint32_t mstatus;
    __asm__ volatile("csrr %0, mstatus" : "=r"(mstatus));
    mstatus |= (1 << 3);  // Global interrupt enable (MIE bit)
    __asm__ volatile("csrw mstatus, %0" : : "r"(mstatus));
}

void __attribute__((interrupt)) trap_handler(void) {
    uint64_t mcause;
    __asm__ volatile("csrr %0, mcause" : "=r"(mcause));
    
    // Check if it's an interrupt (MSB set)
    if (mcause & 0x8000000000000000) {
        uint32_t cause = mcause & 0xFF;
        
        if (cause == CAUSE_MACHINE_TIMER) {
            // Handle timer interrupt
            trap_handler_timer(mcause,0,0);
            
        } else if (cause == CAUSE_MACHINE_EXTERNAL) {
            // Get the interrupt number from PLIC
                // Handle UART interrupt
                uart_interrupt_handler();
            // Complete the interrupt in PLIC
            //plic_complete_interrupt(irq);
        }
    }
}