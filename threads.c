#include "threads.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "timer.h"
#define MAX_NAME_LENGTH 100
static thread_t processus[NB_PROCESSUS];
static thread_t * actif;
static uint32_t prochain_pid = 0 ;

void idle()
{
    for (;;) {
        enable_it();
        hlt();
        disable_it();
    }
}

void proc1(void)
{   affiche_etats();
    for (int32_t i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        dors(2);
        
    }
}

void proc2(void)
{   
  affiche_etats();

    for (int32_t i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        dors(2);
        
    }
}

void proc3(void)
{
  affiche_etats();
    for (int32_t i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        dors(2);
        
    }
}

void init_proc(void) {
    prochain_pid = 0;

    // Initialisation du processus idle
    processus[0].name = "idle";
    processus[0].pid = 0;
    processus[0].state = ELU; // Le processus idle peut être en état ELU ou autre
    processus[0].wake_up_time = 0;
    memset(processus[0].context, 0, sizeof(processus[0].context));
    
    // Initialisation du contexte pour `proc_launcher`
    processus[0].context[0] = (uint64_t) proc_launcher;  // Entrée pour `proc_launcher`
    processus[0].context[1] = (uint64_t) (processus[0].stack + STACK_SIZE); // Pointeur de pile
    processus[0].context[2] = (uint64_t) fin_processus;  // La fonction à appeler dans `proc_launcher`
    
    actif = &processus[0]; // Le processus idle est le processus actif au début

    // Réinitialisation des autres processus
    for (int i = 1; i < NB_PROCESSUS; i++) {
        processus[i].name = NULL;
        processus[i].pid = -1;
        processus[i].state = ENDORMI; // Par défaut, tous les processus sont endormis
        memset(processus[i].context, 0, sizeof(processus[i].context));

        // Initialisation du contexte pour `proc_launcher` pour les nouveaux processus
        processus[i].context[0] = (uint64_t) proc_launcher;  // Entrée pour `proc_launcher`
        processus[i].context[1] = (uint64_t) (processus[i].stack + STACK_SIZE); // Pointeur de pile
        processus[i].context[2] = (uint64_t) fin_processus;  // Fonction de fin automatique
    }
}




void ordonnance(void) {
    if (actif != NULL && actif->state != ENDORMI && actif->state != MORT ) {
        actif->state = ACTIVABLE; // Marquer le processus actuel comme activable
    }

    int32_t pid_courant = (actif != NULL) ? actif->pid : -1;

    // Vérifier les processus endormis
    uint32_t current_time = nbr_secondes();
    for (int i = 0; i < NB_PROCESSUS; i++) {
        if (processus[i].state == ENDORMI && processus[i].wake_up_time == current_time) {
            processus[i].state = ACTIVABLE; // Réveiller le processus
        }
    }

    // Rechercher le prochain processus activable
    for (int i = 1; i <= NB_PROCESSUS; i++) {
        int pid = (pid_courant + i) % NB_PROCESSUS;

        if (processus[pid].state == ACTIVABLE) {
            processus[pid].state = ELU; // Marquer le processus comme élu
            thread_t *ancien = actif;
            actif = &processus[pid];
            ctx_sw(ancien->context, actif->context); // Changement de contexte
            return;
        }
    }

    // Si aucun processus activable, revenir à idle
    actif = &processus[0];
    actif->state = ELU;
    ctx_sw(NULL, actif->context);
}



int32_t  cree_processus(void (*code)(void), char *nom) {

    if (prochain_pid >= NB_PROCESSUS) {
        return -1; // Plus de place pour créer un nouveau processus
    }
    int32_t pid;
    prochain_pid++;
    pid = prochain_pid;
    processus[pid].pid = pid;
    processus[pid].name = nom;
    processus[pid].state = ACTIVABLE;
    processus[pid].context[1] = (uint64_t)(processus[pid].stack + STACK_SIZE );
    processus[pid].context[0] = (uint64_t) proc_launcher;
    processus[pid].context[2] = (uint64_t) code;

    return pid; 

}

int32_t mon_pid(void) {
    return actif->pid;
}

char *mon_nom(void) {
    return actif->name;
}

void dors(uint32_t nbr_secs){
    uint32_t current_time = nbr_secondes();
    uint32_t run_time = current_time + nbr_secs ;
    processus[actif->pid].wake_up_time = run_time;
    processus[actif->pid].state = ENDORMI;
    ordonnance();
}



void fin_processus(void){
    if (actif->pid == 0){ // idle ne se termine pas
        return;
    } 
    actif->state = MORT;
    ordonnance();
}


void affiche_etats(void) {
    for (int i = 0; i < NB_PROCESSUS; i++) {
        const char *etat_str;
        switch (processus[i].state) {
        case ELU: etat_str = "ELU"; break;
        case ACTIVABLE: etat_str = "ACTIVABLE"; break;
        case ENDORMI: etat_str = "ENDORMI"; break;
        case MORT: etat_str = "MORT"; break;
        default: etat_str = "INCONNU"; break;
        }
        printf("  PID %d (%s) : %s\n", processus[i].pid, processus[i].name, etat_str);
    }
}


void proc_launcher(void (*proc)(void)) {
    proc();           
    fin_processus();  }
