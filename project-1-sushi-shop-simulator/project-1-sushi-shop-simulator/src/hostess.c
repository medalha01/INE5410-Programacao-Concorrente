#include <stdlib.h>

#include "hostess.h"
#include "globals.h"
#include "args.h"


int hostess_check_for_a_free_conveyor_seat() {
    /* 
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO HOSTESS.
        NOTAS:
        1.  ✅ O HOSTESS DEVE FICAR EM ESPERA ATÉ QUE UMA VAGA SEJA LIBERADA NA ESTEIRA.
        2.  ✅ A VAGA INICIAL (conveyor->_seats[0]) É RESERVADA PARA O SUSHI CHEF.
        3.  ✅ NÃO REMOVA OS PRINTS, NEM O `msleep()` DE DENTRO DO WHILE LOOP.
        4.  ✅ O RETORNO DESSA FUNÇÃO É O ÍNDICE DO CONVEYOR INDICANDO UM ASSENTO LIVRE.
        5.  ✅ CUIDADO COM PROBLEMAS DE SINCRONIZAÇÃO!
    */
    conveyor_belt_t* conveyor = globals_get_conveyor_belt();
    virtual_clock_t* virtual_clock = globals_get_virtual_clock();
    pthread_mutex_t* seat_mutexes = globals_get_seat_mutexes();

    print_virtual_time(globals_get_virtual_clock());
    fprintf(stdout, GREEN "[INFO]" NO_COLOR " O Hostess está procurando por um assento livre...\n");
    print_conveyor_belt(conveyor);

    /* ✅ 5 - Usando um semáforo, faz com que o while(TRUE) seja desnecessário,
    uma vez que entra para a fila e espera o contador do semáforo ter um valor maior que 0
    sem busy waiting */
    sem_t *sem = globals_get_seats_sem();
    sem_wait(sem);
    for (int i=1; i<conveyor->_size; i++) {
        
        pthread_mutex_lock(&seat_mutexes[i]);
        if (conveyor->_seats[i] == -1) {  // Atenção à regra! (-1 = livre, 0 = sushi_chef, 1 = customer)
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " O Hostess encontrou o assento %d livre para o próximo cliente!\n", i);
            return i;
        }
        pthread_mutex_unlock(&seat_mutexes[i]);
    }
    // ✅ 3
    msleep(120000/virtual_clock->clock_speed_multiplier);  // Não remova esse sleep!
}

void hostess_guide_first_in_line_customer_to_conveyor_seat(int seat) {
    /* 
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO HOSTESS.
        NOTAS:
        1.  ✅ NESSA FUNÇÃO É PAPEL DO HOSTESS GARANTIR 3 ATUALIZAÇÕES:
            ✅ a. O valor _seat_position do cliente em questão deve ser atualizado
            ✅ b. O valor do assento em questão no conveyor_belt global deve ser atualizado 
               (-1 = vazio, 0 = sushi_chef, 1 = cliente)
            ✅ c. O cliente em questão deve ser removido da fila global
        2.  ✅ CUIDADO COM PROBLEMAS DE SINCRONIZAÇÃO!
        3.  ✅ NÃO REMOVA OS PRINTS!
    */
    conveyor_belt_t* conveyor = globals_get_conveyor_belt();
    queue_t* queue = globals_get_queue();
    pthread_mutex_t* seat_mutexes = globals_get_seat_mutexes();

    // ✅ 1
    /* ✅ 2 - o problem de sincronização já é resolvido com o uso do semáforo e com a lógica
    de saída e de entrada no assento por parte do customer e da hostess */
    customer_t* customer = queue_remove(queue);
    conveyor->_seats[seat] = 1;
    customer->_seat_position=seat;
    sem_post(&customer->_customer_sem);
    pthread_mutex_unlock(&seat_mutexes[seat]);

    // ✅ 3
    print_virtual_time(globals_get_virtual_clock());
    fprintf(stdout, GREEN "[INFO]" NO_COLOR " O Hostess levou o cliente %d para o assento %d!\n", customer->_id, seat);
    print_conveyor_belt(conveyor);    
}

void* hostess_run() {
    /* 
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO HOSTESS.
        NOTAS:
        1.  ✅ O HOSTESS DEVE FUNCIONAR EM LOOP, RETIRANDO CLIENTES DA FILA GLOBAL E ADICIONANDO-OS NA
            ESTEIRA GLOBAL CONFORME VAGAS SÃO LIBERADAS.
        2.  ✅ QUANDO O SUSHI SHOP FECHAR, O HOSTESS DEVE PARAR DE GUIAR NOVOS CLIENTES DA FILA PARA 
            A ESTEIRA, E ESVAZIAR A FILA GLOBAL, FINALIZANDO OS CLIENTES EM ESPERA.
        3.  ✅ CUIDADO COM PROBLEMAS DE SINCRONIZAÇÃO!
        4.  ✅ NÃO REMOVA OS PRINTS!
    */
    virtual_clock_t* virtual_clock = globals_get_virtual_clock();
    queue_t* queue = globals_get_queue();

    // ✅ 1 e 2
    while (virtual_clock->current_time < virtual_clock->closing_time) { 
        if (queue->_length > 0) { // TODO: fix
            int seat = hostess_check_for_a_free_conveyor_seat();
            hostess_guide_first_in_line_customer_to_conveyor_seat(seat);
        }
        msleep(3000/virtual_clock->clock_speed_multiplier);  // Não remova esse sleep!
    }

    // ✅ 2 
    queue_finalize(queue);
    pthread_exit(NULL);
}

hostess_t* hostess_init() {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    hostess_t* self = malloc(sizeof(hostess_t));
    if (self == NULL) {
        fprintf(stdout, RED "[ERROR] Bad malloc() at `hostess_t* hostess_init()`.\n" NO_COLOR);
        exit(EXIT_FAILURE);
    }
    pthread_create(&self->thread, NULL, hostess_run, NULL);
    return self;
}

void hostess_finalize(hostess_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    pthread_join(self->thread, NULL);
    free(self);
}
