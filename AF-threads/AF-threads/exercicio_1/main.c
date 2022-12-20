#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

//                 (main)      
//                    |
//    +----------+----+------------+
//    |          |                 |   
// worker_1   worker_2   ....   worker_n


// ~~~ argumentos (argc, argv) ~~~
// ./program n_threads

// ~~~ printfs  ~~~
// main thread (após término das threads filhas): "Contador: %d\n"
// main thread (após término das threads filhas): "Esperado: %d\n"

// Obs:
// - pai deve criar n_threds (argv[1]) worker threads 
// - cada thread deve incrementar contador_global (operador ++) n_loops vezes
// - pai deve esperar pelas worker threads  antes de imprimir!


int contador_global = 0;

/* declaring a struct ARGS so that all the arguments 
can be passed into the thread function */
typedef struct ARGS {
    int *counter;
    int n;
} ARGS;

void* thread(void* allargs) {
    /* due to pthread_create, allargs must be void*,
    therefore, it needs to be casted back into ARGS* */
    ARGS *arg = (ARGS *)allargs; 
    for (int j = 0; j < arg->n; j++)
        (*(arg->counter))++; 
    pthread_exit(arg->counter);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("n_threads é obrigatório!\n");
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);

    pthread_t all_threads[n_threads]; // array to store the unique thread ids
    ARGS allargs = { &contador_global, n_loops };

    for (int i = 0; i < n_threads; i++)
        pthread_create(&all_threads[i], NULL, thread, (void *) &allargs); 
    
    /* all threads need to be joined after their operations are done
    this is due to them having to share the same global variable */
    for (int z = 0; z < n_threads; z++)
        pthread_join(all_threads[z], NULL);
    
    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads*n_loops);
    return 0;
}
