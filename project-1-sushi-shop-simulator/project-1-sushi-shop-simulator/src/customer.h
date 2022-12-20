#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__

#include <pthread.h>
#include "args.h"
#include "menu.h"
#include "conveyor_belt.h"
#include <semaphore.h>

/**
 * @brief Cliente do Sushi Shop.
*/
typedef struct customer {
    int _id;
    int _wishes[5];
    int _seat_position;
    pthread_t thread;
    sem_t _customer_sem;
} customer_t;

customer_t* customer_init();
void* customer_run(void* arg);
void customer_finalize(customer_t* self);

void print_customer(customer_t* self);

int customer_pick_food(customer_t* self, int food_slot, int j);
void customer_eat(customer_t* self, enum menu_item food);
void customer_leave(customer_t* self);

#endif  //__CUSTOMER_H__
