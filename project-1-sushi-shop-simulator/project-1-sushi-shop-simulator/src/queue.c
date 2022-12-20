#include <stdlib.h>
#include <time.h>

#include "virtual_clock.h"
#include "globals.h"
#include "queue.h"
#include "args.h"


void* queue_run(void *arg) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    queue_t* self = (queue_t*) arg;
    virtual_clock_t* clock = globals_get_virtual_clock();
    while (TRUE) {
        customer_t* customer = customer_init();
        sem_init(&customer->_customer_sem, 0, 0); // 🤠🤠🤠🤠🤠🤠🤠
        queue_insert(self, customer);
        print_virtual_time(clock);
        fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d arrived at the Sushi Shop queue!\n", customer->_id);
        print_queue(self);
        msleep((rand() % 120000)/clock->clock_speed_multiplier);
    }
}

queue_t* queue_insert(queue_t *self, customer_t* customer) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    if (self == NULL) {
        self = queue_init(self);
    }
    struct queue_item *item = (struct queue_item *)malloc(sizeof(struct queue_item));
    item->_customer = customer;
    item->_next = NULL;
    self->_length = self->_length + 1;
    if (self->_length == 1) {
        self->_first = item;
        self->_last = item;
    }
    else {
        self->_last->_next = item;
        self->_last = item;
    }
    return self;
}

customer_t* queue_remove(queue_t *self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    if (self->_length > 0) {
        customer_t* customer = NULL;
        struct queue_item* item = NULL;
        customer = self->_first->_customer;
        item = self->_first;
        self->_first = self->_first->_next;
        self->_length = self->_length - 1;
        free(item);
        return customer;
    }
    return NULL;
}

queue_t* queue_init() {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    queue_t *self = malloc(sizeof(queue_t));
    if (self == NULL) {
        fprintf(stdout, RED "[ERROR] Bad malloc() at `queue_t* queue_init()`.\n" NO_COLOR);
        exit(EXIT_FAILURE);
    }
    self->_first = NULL;
    self->_last = NULL;
    self->_length = 0;
    pthread_create(&self->thread, NULL, queue_run, (void *) self);
    return self;
}

void queue_finalize(queue_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    struct queue_item *item = NULL;
    for (int i=0; i<self->_length; i=i+1) {
        item = self->_first;
        self->_first = self->_first->_next;
        customer_finalize(item->_customer);
        free(item);
    }
    pthread_join(self->thread, NULL);
    free(self);
}

void print_queue(queue_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    print_virtual_time(globals_get_virtual_clock());
    fprintf(stdout, BROWN "[DEBUG] Customer Queue " NO_COLOR "{\n");
    fprintf(stdout, BROWN "    _length" NO_COLOR ": %d\n", self->_length);
    fprintf(stdout, NO_COLOR "}\n" NO_COLOR);
}
