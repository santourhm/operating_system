#include <inttypes.h>
#include <stdio.h>
#include <cpu.h>
#include "font.h"
#include "string.h"
#include "console.h"
#include "timer.h"
#include "threads.h"
#include "uart.h"
#include "interrupt.h"
#include <cep_platform.h>
#include "shell.h"
extern void mon_traitant(void);


void run_tests_console_print() {
    console_putbytes("Hello World!", 12);      // Test simple text
    console_putbytes("\nThis is a new line.", 21); // Test line feed
    console_putbytes("\tTabulation Test", 18); // Test tabulation
    console_putbytes("\bBackspace Test", 16);  // Test backspace
    console_putbytes("\rCarriage return\n", 18); // Test carriage return and newline
    console_putbytes("Scroll Test Start", 18); // Filling lines to scroll
    for (int i = 0; i < 90; i++) {
        char buffer[4];
        sprintf(buffer, "%d\n", i); // Create a number to scroll the screen
        console_putbytes(buffer, strlen(buffer)); // Test scrolling with numbers
    }
    char * buffer = "ajouter la fonction run_tests_console_clear() pour tester nettoyage";
    console_putbytes(buffer,strlen(buffer)); // Test form feed (clear screen)
}

void run_tests_console_clear(){
    console_putbytes("\fClear Screen Test", 20);
}

void tests_prof(){
    printf("Truc\nSalut\n1\t2\t3\nAB\bC\nBEEF\rRABBIT\n");
    for (int i=0;i<84;i++) {printf("%d\n",i);}
    printf("Hello\n");
    printf("\f");
} 

void test_ordonnance(){
    init_proc();
    cree_processus(proc1, "proc1");
    cree_processus(proc2, "proc2");
    cree_processus(proc3, "proc3"); 
    idle(); 

}

// test uart keywords


void kernel_start(void){
    
    
    uart_init();
    uart_puts("Tapez un caractère :\n");
    //init_traitant_timer(mon_traitant);
    init_interrupt_system();
    enable_timer();
    shell_init();
    shell_run();
   
   
    while (1) {
       hlt();
    }}

void test_timer(void){
    init_traitant_timer(mon_traitant);
    enable_timer();
    enable_it();
}

/* void kernel_start(void){   
   run_tests_console_print();
   test_timer();
   test_ordonnance();
   
    while (1) {
       
       hlt();
    }}
  */







