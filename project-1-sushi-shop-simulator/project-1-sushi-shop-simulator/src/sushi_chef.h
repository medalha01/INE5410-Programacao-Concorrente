#ifndef __SUSHI_CHEF_H__
#define __SUSHI_CHEF_H__

#include <pthread.h>
#include "args.h"
#include "menu.h"
#include "conveyor_belt.h"


/**
 * @brief Estrutura encapsulando um Sushi chef.
*/
typedef struct sushi_chef {
    int _id;
    int _seat_position;
    pthread_t thread;
} sushi_chef_t;

void sushi_chef_seat(sushi_chef_t* self);
void sushi_chef_prepare_food(sushi_chef_t* self, enum menu_item menu_item);
void sushi_chef_place_food(sushi_chef_t* self, enum menu_item dish);
void sushi_chef_leave(sushi_chef_t* self);

sushi_chef_t* sushi_chef_init();
void* sushi_chef_run(void* arg);
void sushi_chef_finalize(sushi_chef_t* self);

void print_sushi_chef(sushi_chef_t* self);

#endif  // __SUSHI_CHEF_H__
