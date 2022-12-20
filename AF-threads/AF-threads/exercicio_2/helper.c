#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*******************************
 *          ATENÇÃO            *
 *   NÃO EDITAR ESSE ARQUIVO   *
 *          ATENÇÃO            *
 *******************************/

// Gera um vetor de tamanho size com valores aleatórios em [0, 10000).
double* generate_vector(int size) {
    double* vector = (double*)malloc(size*sizeof(double));
    for (int i = 0; i < size; ++i) 
        vector[i] = rand() % 10000;
    return vector;
}

/*******************************
 *          ATENÇÃO            *
 *   NÃO EDITAR ESSE ARQUIVO   *
 *          ATENÇÃO            *
 *******************************/

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// |         usado como 2o retorno! <-----+
// v                                      v
double* load_vector(const char* filename, int* out_size) {
    // O corpo dessa função tem pouca coisa interessante. Não é preciso alterar
    // nem entender como a função funciona, só precisa acreditar na interface

    int size = 0;
    double* vector;
    if (strncmp(filename, "gen:", 4) == 0) {
        if (sscanf(filename+4, "%d", &size) <= 0) 
            return NULL;
        vector = generate_vector(size);
    } else {
        FILE* f = fopen(filename, "r");
        if (!f) return NULL;
        int capacity = 10;
        vector = (double*)malloc(capacity*sizeof(double));
        double value = 0;
        //                 +-----> "long float": double
        //                 v
        while (fscanf(f, "%lf", &value) > 0) {
            if (size >= capacity) {
                capacity = 2*capacity;
                vector = realloc(vector, capacity*sizeof(double));
            }
            vector[size++] = value;
        }
        fclose(f);
    }
    if (out_size) *out_size = size;
    return vector;
}

/*******************************
 *          ATENÇÃO            *
 *   NÃO EDITAR ESSE ARQUIVO   *
 *          ATENÇÃO            *
 *******************************/

// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size. Se silent for diferente de zero, não irá imprimir
// resultado na tela
void avaliar(double* a, double* b, double* c, int size) {
    for (int i = 0; i < size; ++i) 
        printf("%s%g", i ? " " : "", c[i]);
    printf("\n");
    int diffs = 0;
    for (int i = 0; i < size; ++i) 
        diffs += a[i] + b[i] != c[i];
    if (diffs) {
        printf("Ops! Vetor c difere de a + b em %d posições\n", diffs);
    } else {
        printf("OK!\n");
    }
}

/*******************************
 *          ATENÇÃO            *
 *   NÃO EDITAR ESSE ARQUIVO   *
 *          ATENÇÃO            *
 *******************************/
