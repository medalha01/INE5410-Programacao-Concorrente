#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "conveyor_belt.h"
#include "virtual_clock.h"
#include "queue.h"
#include "args.h"
#include <semaphore.h>


// pode dar ruim com o globals_get_conveyor_belt() que está mais pro final daqui

extern conveyor_belt_t* globals_get_conveyor_belt();
extern void globals_set_conveyor_belt();

/**
 * @brief Inicializa o array de mutexes com malloc
 *
 * @param size
 */
extern void globals_set_seat_mutexes(int size);

/**
 * @brief Retorna um ponteiro para o array de mutexes
 *
 * @return pthread_mutex_t*
 */
extern pthread_mutex_t *globals_get_seat_mutexes();

/**
 * @brief Inicializa o array de mutexes com malloc
 *
 * @param size
 */
extern void globals_set_food_slots_mutexes(int size);

/**
 * @brief Retorna um ponteiro para o array de mutexes
 *
 * @return pthread_mutex_t*
 */
extern pthread_mutex_t *globals_get_food_slots_mutexes();

/**
 * @brief Inicia um semáforo de assentos (de modo global)
 *
 * @param sem_t
 */
extern void globals_set_seats_sem(sem_t sem);

/**
 * @brief Inicia um semáforo dos slots de comida (de modo global)
 *
 * @param sem_t
 */
extern void globals_set_food_sem(sem_t *sem);

/**
 * @brief Retorna um semáforo de assentos (de modo global)
 *
 * @return sem_t*
 */
extern sem_t* globals_get_seats_sem();

/**
 * @brief Retorna um semáforo de slot de comida(de modo global)
 *
 * @return sem_t*
 */
extern sem_t *globals_get_food_sem();

/**
 * @brief Inicia um relógio virtual (de modo global)
 *
 * @param virtual_clock
 */
extern void globals_set_virtual_clock(virtual_clock_t *virtual_clock);

/**
 * @brief Retorna um relógio virtual (de modo global)
 *
 * @return virtual_clock_t*
 */
extern virtual_clock_t *globals_get_virtual_clock();

/**
 * @brief Inicia uma esteira de sushi (de modo global).
 *
 * @param conveyor_belt
 */
//extern void globals_set_conveyor_belt(conveyor_belt_t *conveyor_belt);

/**
 * @brief Retorna uma esteira de sushi (de modo global)
 *
 * @return conveyor_belt_t*
 */
//extern conveyor_belt_t *globals_get_conveyor_belt();

/**
 * @brief Inicia uma fila (de modo global)
 *
 * @param queue
 */
extern void globals_set_queue(queue_t *queue);

/**
 * @brief Retorna uma fila (de modo global)
 *
 * @return queue_t*
 */
extern queue_t *globals_get_queue();

/**
 * @brief Finaliza todas as variáveis globais.
 *
 */
extern void globals_finalize();

#endif // __GLOBALS_H__
