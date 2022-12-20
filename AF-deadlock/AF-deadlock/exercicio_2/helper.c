#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "banco.h"

#define MILLION 1000000L

conta_t contas[NUM_CONTAS];

//    +============================+
//    |                            |
// ~~~| NÃO MODIFIQUE ESSE ARQUIVO |~~~
//    |                            |
//    +============================+

//Definida em main.c
void transferir(conta_t *origem, conta_t *destino, double valor);

void transferir_unsafe(conta_t* origem, conta_t* destino, double valor) {
    printf("Transferindo %.2f da conta %d para %d\n", valor, origem->id, destino->id);
    fflush(stdout);

    origem->saldo -= valor;
    destino->saldo += valor;
    
    struct timespec ts = {0, 200*MILLION};
    nanosleep(&ts, NULL);
}

void *caixa_func(void *arg) {
    int conta1, conta2;
  
    for(int i = 0; i < TOTAL_TRANSFERENCIAS; i++) {
        // Escolhe duas contas diferentes aleatoriamente
        conta1 = rand() % NUM_CONTAS;
        conta2 = rand() % NUM_CONTAS;
        while(conta1 == conta2) conta2 = rand() % NUM_CONTAS;
        // Realiza a transferencia
        transferir(&contas[conta1], &contas[conta2], (double) 100.0 + (rand() % 900));
    }

    return NULL;
}
