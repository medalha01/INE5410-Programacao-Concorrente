#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MILLION 1000000L
extern int total_computado;

//    +============================+
//    |                            |
// ~~~| NÃO MODIFIQUE ESSE ARQUIVO |~~~
//    |                            |
//    +============================+

static unsigned seeds[] = {0, 0};

static int operacao_worker(int worker) {
    unsigned* seedp = &seeds[worker-1];
    if (!*seedp) *seedp = time(NULL);
    int value = 1+rand_r(seedp)%500;
    struct timespec ts = {0, value*MILLION};
    nanosleep(&ts, NULL);
    printf("Worker %d produziu %d\n", worker, value);
    return value;
}

int operacao_worker1() {return operacao_worker(1);}
int operacao_worker2() {return operacao_worker(2);}

void imprime_resultado(int total, int* lista, int size) {
    printf("Total das operações: %d\n", total);
}

