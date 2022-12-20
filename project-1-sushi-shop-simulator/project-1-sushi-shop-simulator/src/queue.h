#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <pthread.h>
#include "customer.h"


struct queue_item {
    customer_t* _customer;  // Ponteiro que aponta para um cliente
    struct queue_item* _next;  // Ponteiro que aponta para o próximo cliente da fila
};

typedef struct queue {
    int _length;  // Tamanho atual da fila
    struct queue_item *_first;  // Primeiro da Fila
    struct queue_item *_last;  // Último da fila
    pthread_t thread;  // Thread que irá ficar gerando clientes novos
} queue_t;

queue_t* queue_insert(queue_t *self, customer_t *customer);
customer_t* queue_remove(queue_t *self);

queue_t* queue_init();
void* queue_run(void *arg);
void queue_finalize(queue_t* self);

void print_queue(queue_t* self);

#endif // __QUEUE_H__
