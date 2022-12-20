#ifndef __CONVEYOR_BELT_H__
#define __CONVEYOR_BELT_H__

#include <pthread.h>
#include "args.h"


#define CONVEYOR_IDLE_PERIOD     30000
#define CONVEYOR_MOVING_PERIOD   5000

#define EMPTY_SLOT               "__"
#define SUSHI_CHEF               "🍳"
#define CUSTOMER                 "😋"

/**
 * @brief Esteira de Sushi do Sushi Shop.
*/
typedef struct conveyor_belt {
    int _size;
    int* _seats;
    int* _food_slots;
    pthread_mutex_t _seats_mutex;
    pthread_mutex_t _food_slots_mutex;
    pthread_t thread;
} conveyor_belt_t;

conveyor_belt_t* conveyor_belt_init(config_t* config);
void* conveyor_belt_run(void* arg);
void conveyor_belt_finalize(conveyor_belt_t* self);

void print_conveyor_belt(conveyor_belt_t* self);

#endif  // __CONVEYOR_BELT_H__
