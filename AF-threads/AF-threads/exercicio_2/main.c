#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

/* definindo o struct que vai ser usado para passar
todos os argumentos necessários à função thread */
typedef struct allargs {
    double* a;
    double* b;
    double* c;     
    int n_loops;          // a_size / n_threads = n_loops
    int* start;           // index do início do for na função thread
    int* remainder_loops; // a_size % n_threads = remainder_loops
} allargs;

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);

void* thread(void* arg);

// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);


int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    //srand(0);        //sempre mesmo valor

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n",
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }
    //Cria vetor do resultado 
    double* c = malloc(a_size*sizeof(double));

    // Calcula com uma thread só. Programador original só deixou a leitura 
    // do argumento e fugiu pro caribe. É essa computação que você precisa 
    // paralelizar

    /* caso o n_threads seja maior que os vetores, 
    diminui n_threads para limitar seu uso */
    if (n_threads > a_size) n_threads = a_size;

    int n_loops = a_size / n_threads;
    int remainder_loops = a_size % n_threads;
    int start = 0;
    pthread_t all_threads[n_threads];
    allargs std_args = { a, b, c, n_loops, &start, &remainder_loops };

    int i;
    for (i = 0; i < n_threads; ++i)   
        pthread_create(&all_threads[i], NULL, thread, (void*) &std_args);

    for (int j = 0; j < n_threads; j++) 
        pthread_join(all_threads[j], NULL);
        
    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, c, a_size);
    

    //Importante: libera memória
    free(a);
    free(b);
    free(c);

    return 0;
}

void* thread(void* arg) {
    allargs* args = (allargs*)arg;

    /* essa variável armazena n_loops se não tiver remainder_loops,
    caso tenha, ela vai armazenar n_loops+1 para distribuir os 
    remainder_loops de forma igualitária para vários threads */
    int temp = args->n_loops;
    if (*(args->remainder_loops) > 0) {
        temp++;
        *(args->remainder_loops) -= 1;
    }
    
    for (int i = *(args->start); i < temp + *(args->start); i++)
        args->c[i] = args->a[i] + args->b[i];
    *(args->start) += temp;
    pthread_exit(NULL);
}
