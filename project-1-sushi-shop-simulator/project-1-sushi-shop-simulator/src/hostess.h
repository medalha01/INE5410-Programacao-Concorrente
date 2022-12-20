#ifndef __HOSTESS_H__
#define __HOSTESS_H__

#include <pthread.h>
#include "customer.h"


/**
 * @brief Estrutura encapsulando um Hostess do sushi shop
*/
typedef struct hostess {
    pthread_t thread;
} hostess_t;

int hostess_check_for_a_free_conveyor_seat();
void hostess_guide_first_in_line_customer_to_conveyor_seat(int seat);

hostess_t* hostess_init();
void* hostess_run();
void hostess_finalize(hostess_t *self);

#endif  // __HOSTESS_H_