#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "virtual_clock.h"


void* virtual_clock_run(void* arg) {
    /* ESSA FUNÇÃO JÁ POSSUÍ A LÓGICA BÁSICA DE FUNCIONAMENTO DO RELÓGIO VIRTUAL */
    virtual_clock_t* self = (virtual_clock_t*) arg;
    while (TRUE) {
        if (self->current_time >= self->closing_time) {
            print_virtual_time(self);
            fprintf(stdout, GREEN "[INFO]" RED " RESTAURANT IS CLOSED!!!\n");
        }
        self->current_time += 1;
        msleep(1000/self->clock_speed_multiplier);
    }
    pthread_exit(NULL);
}

virtual_clock_t* virtual_clock_init(config_t* config) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    virtual_clock_t* self = malloc(sizeof(virtual_clock_t));
    if (self == NULL) {
        fprintf(stdout, RED "[ERROR] Bad malloc() at `virtual_clock_t* virtual_clock_init()`.\n" NO_COLOR);
        exit(EXIT_FAILURE);
    }
    self->clock_speed_multiplier = config->clock_speed_multiplier;
    self->opening_time = 3600 * config->opening_time;
    self->closing_time = 3600 * config->closing_time;
    self->current_time = 3600 * config->opening_time;
    pthread_create(&self->thread, NULL, virtual_clock_run, (void *) self);
    return self;
}

void virtual_clock_finalize(virtual_clock_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    pthread_join(self->thread, NULL);
    free(self);
}

unsigned int read_hours(unsigned int value) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    return value / HOUR;
}

unsigned int read_minutes(unsigned int value) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    return (value / MINUTE) % MINUTE;
}

unsigned int read_seconds(unsigned int value) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    return value % MINUTE;
}

unsigned int read_ms(unsigned int value) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    return value % MS;
}

void print_virtual_time(virtual_clock_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    fprintf(stdout, MAGENTA "[%02dh%02dm%02ds %04dms] " NO_COLOR, read_hours(self->current_time), read_minutes(self->current_time), read_seconds(self->current_time), read_ms(self->current_time));
}

int msleep(long msec) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    struct timespec ts;
    int res;
    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);
    return res;
}
