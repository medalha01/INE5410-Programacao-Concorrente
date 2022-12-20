#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "args.h"
#include "customer.h"
#include "sushi_chef.h"
#include "hostess.h"
#include "globals.h"


int main (int argc, char** argv) {
    /* Read command line options */
    config_t config = parse(argc, argv);

    /* Setup random seed */
    srand(time(0));

    /* Setup simulation virtual clock */
    globals_set_virtual_clock(virtual_clock_init(&config));

    /* Setup sushi conveyor belt */
    //globals_set_conveyor_belt(
    conveyor_belt_init(&config);

    /* COISAS QUE ADICIONAMOS */
    
    // ARRUMA ISSO 🤠 
    conveyor_belt_t* conveyor = globals_get_conveyor_belt();
    sem_t *sem = globals_get_seats_sem();
    sem_init(sem, 0, conveyor->_size);
    
    globals_set_seat_mutexes(conveyor->_size);
    pthread_mutex_t* seat_mutexes = globals_get_seat_mutexes();
    for (int i = 0; i < conveyor->_size; i++)
        pthread_mutex_init(&seat_mutexes[i], NULL);

    globals_set_food_slots_mutexes(conveyor->_size);
    pthread_mutex_t* food_mutexes = globals_get_food_slots_mutexes();
    for (int i = 0; i < conveyor->_size; i++)
        pthread_mutex_init(&food_mutexes[i], NULL);


    /* Init the Sushi Chef */
    sushi_chef_t** chefs = (sushi_chef_t**)malloc(sizeof(sushi_chef_t)*config.sushi_chefs);    
    
    for (int i = 0; i < config.sushi_chefs; i++)
        chefs[i] = sushi_chef_init();

    /* Setup customer queue */
    globals_set_queue(queue_init());

    /* Init the Hostess */
    hostess_t* hostess = hostess_init();

    /* Join threads and free used memory */
    hostess_finalize(hostess);
    
    for (int i = 0; i < config.sushi_chefs; i++)
        sushi_chef_finalize(chefs[i]);
    
    globals_finalize();
    /* FIM DO CÓDIGO QUE NÓS COLOCAMOS */


    return EXIT_SUCCESS;
}
