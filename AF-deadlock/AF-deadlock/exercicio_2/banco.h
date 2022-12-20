#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <pthread.h>

#define NUM_CAIXAS 4
#define NUM_CONTAS 5
#define TOTAL_TRANSFERENCIAS 100

typedef struct {
    int id;
    double saldo;
    pthread_mutex_t mutex;
} conta_t;

// Global com as contas. 
// Usada em helper.c e main.c
extern conta_t contas[NUM_CONTAS];
//^^^^
//+-----> extern evita a definição de uma global. Aqui apenas declaramos que a 
//        global existe. A única definição ocorre em helper.c. Sem o extern, 
//        cada arquivo iria tentar declarar a própria global

#endif /*__CONFIG_H__*/
