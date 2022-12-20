#include <stdlib.h>

#include "globals.h"

/*
    VOCÊ DEVE CRIAR VARIÁVEIS GLOBAIS PARA ARMAZENAR DADOS SOBRE A SIMULAÇÃO.
    NOTAS:
    1.  OS SEGUINTES DADOS DEVEM SER ARMAZENADOS E PRINTADOS AO FIM DA SIMULAÇÃO:
        a. Quantidade de clientes que sentaram-se no conveyor e consumiram todos os itens desejados
        b. Quantidades produzidas de cada alimento
        c. Quantidades consumidas de cada alimento
    2.  SIGA OS EXEMPLOS DE VARIÁVEIS GLOBAIS JÁ EXISTENTES NESSE ARQUIVO PARA CRIAR AS NOVAS.
*/

virtual_clock_t *global_virtual_clock = NULL;
queue_t *global_queue = NULL;

/* CÓDIGO QUE ADICIONAMOS COMEÇA AQUI */
sem_t seats_sem;
pthread_mutex_t *seat_mutexes = NULL;
sem_t *food_sem = NULL;
pthread_mutex_t *food_slot_mutexes = NULL;
conveyor_belt_t* conveyor_belt = NULL;

// atenção ao global_conveyor_belt, pode dar BO aqui

conveyor_belt_t* globals_get_conveyor_belt() 
{
    return conveyor_belt;
} 

void globals_set_conveyor_belt() 
{
    conveyor_belt = (conveyor_belt_t*)malloc(sizeof(conveyor_belt_t));
}

void globals_set_seats_sem(sem_t sem)
{
    seats_sem = sem;
}

sem_t* globals_get_seats_sem()
{
    return &seats_sem;
}

void globals_set_food_sem(sem_t *sem)
{
    food_sem = sem;
}

sem_t *globals_get_food_sem()
{
    return food_sem;
}

void globals_set_food_slots_mutexes(int size)
{
    food_slot_mutexes = malloc(size * sizeof(pthread_mutex_t));
}

pthread_mutex_t *globals_get_food_slots_mutexes()
{
    return food_slot_mutexes;
}

void globals_set_seat_mutexes(int size)
{
    seat_mutexes = malloc(size * sizeof(pthread_mutex_t));
}

pthread_mutex_t *globals_get_seat_mutexes()
{
    return seat_mutexes;
}
/* CÓDIGO QUE ADICIONAMOS TERMINA AQUI */

void globals_set_virtual_clock(virtual_clock_t *virtual_clock)
{
    global_virtual_clock = virtual_clock;
}

virtual_clock_t *globals_get_virtual_clock()
{
    return global_virtual_clock;
}

void globals_set_queue(queue_t *queue)
{
    global_queue = queue;
}

queue_t *globals_get_queue()
{
    return global_queue;
}

/**
 * @brief Finaliza todas as variáveis globais.
 * Se criar alguma variável global que faça uso de mallocs, lembre-se sempre
 * de usar o free dentro dessa função.
 */
void globals_finalize()
{
    virtual_clock_finalize(global_virtual_clock);
    free(food_slot_mutexes);
    free(seat_mutexes);
    conveyor_belt_finalize(conveyor_belt);
}
