#include "timer.h"
#define CLINT_TIMER_CMP 0x02004000
#define CLINT_TIMER 0x0200bff8
#define TIME_FREQ 10000000
#define IT_FREQ 20
#include "threads.h"

static uint32_t IntElapesed = 0;


void print_timer(char * buffer){

    // Affiche le buffer au-dessus de l'écran
    uint32_t x =  1200;
    uint32_t y = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        ecrit_car(x, y, buffer[i], WRITING_COLOR);
        x += 8; // Avance de 8 pixels pour le caractère suivant
    } 
}


void trap_handler_timer(uint64_t mcause,uint64_t mie, uint64_t mip ) {
    // Vérifier que la cause est bien une interruption de type timer (mcause = 7 avec le bit 63 à 1)
    if ((mcause & 0xFF ) == 7) {
        
        // Lire le temps actuel à partir de l'adresse CLINT_TIMER
        uint64_t current_time = *(volatile uint64_t *)CLINT_TIMER;

        // Programmer le prochain déclenchement de l'interruption timer
        *(volatile uint64_t *)CLINT_TIMER_CMP = current_time + (TIME_FREQ / IT_FREQ);

        // Incrémenter le compteur de secondes écoulées
        IntElapesed++;

        // Calculer les heures, minutes et secondes à afficher
        uint32_t elapsed_seconds = IntElapesed/IT_FREQ;
        uint32_t hours = elapsed_seconds / 3600;
        uint32_t minutes = (elapsed_seconds % 3600) / 60;
        uint32_t secs = elapsed_seconds % 60;
        
        // Mettre à jour l'affichage du timer
       
        char buffer[18];
        sprintf(buffer, "[%02u:%02u:%02u]", hours, minutes, secs);
        print_timer(buffer); 
        //ordonnance();        
    }
}


void init_traitant_timer(void (*traitant)(void)) {
    __asm__("csrw mtvec, %0" : : "r"(traitant));
}

void enable_timer() {
    // Lire la valeur actuelle du timer à partir de CLINT_TIMER
    uint64_t current_time = *(volatile uint64_t *)CLINT_TIMER;

    // Programmer la première interruption en ajoutant le délai souhaité (IT_FREQ)
    *(volatile uint64_t *)CLINT_TIMER_CMP = current_time + (TIME_FREQ / IT_FREQ);

    // Activer les interruptions timer dans le registre mie (bit 7)
    uint64_t mie;
    __asm__("csrr %0, mie" : "=r"(mie)); 
    mie |= (1 << 7); // Activer le bit 7 pour autoriser les interruptions timer
    __asm__("csrw mie, %0" :: "r"(mie)); // Écrire la nouvelle valeur dans mie
}



uint32_t nbr_secondes(void){
    return (uint32_t) IntElapesed/IT_FREQ;
}
