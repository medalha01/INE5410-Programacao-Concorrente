#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia se o prod_escalar é o produto escalar dos vetores a e b. Assume-se
// que ambos a e b sejam vetores de tamanho size.
void avaliar(double* a, double* b, int size, double prod_escalar);

typedef struct {
    double *array_a, *array_b;
    double thread_result;
    unsigned int start, end;
} array_infos;

void* mult_indexes(void* arg){
    array_infos *info = (array_infos*)arg;
    for (int i = info -> start; i <= info -> end; i++){
        info -> thread_result += (info -> array_a[i]) * (info -> array_b[i]);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

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

    if (a_size < n_threads) n_threads = a_size;

    pthread_t threads[n_threads];
    array_infos infos[n_threads];


    //Calcula produto escalar. Paralelize essa parte
    double result = 0;  

    unsigned int index_amt = a_size / n_threads;
    unsigned int rest = a_size % n_threads;
    for (int i = 0; i < n_threads; i++){
        infos[i].array_a = a;
        infos[i].array_b = b;
        infos[i].thread_result = 0;
        // Dividindo igualmente as tarefas entre as threads
        if (rest != 0)
            if (i < rest){
                infos[i].start = (i * (index_amt + 1));
                infos[i].end = infos[i].start + index_amt;
            } else { 
                infos[i].start = (rest + i * index_amt);
                infos[i].end = infos[i].start + index_amt - 1;
            }
        else{
            infos[i].start = i * index_amt;
            infos[i].end = infos[i].start + index_amt - 1;
        }
    }    

    for (int i = 0; i < n_threads; ++i)
        pthread_create(&threads[i], NULL, mult_indexes, (void*)&infos[i]);
    
    for (int i = 0; i < n_threads; i++){
        pthread_join(threads[i], NULL);
        result += infos[i].thread_result;
    }
    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, a_size, result);

    //Libera memória
    free(a);
    free(b);

    return 0;
}
