#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// Gera um vetor de tamanho size com valores aleatórios em [0, 10000).
double* generate_vector(int size) {
    double* vector = (double*)malloc(size*sizeof(double));
    for (int i = 0; i < size; ++i) 
        vector[i] = rand() % 10000;
    return vector;
}

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

// Avalia se o prod_escalar é o produto escalar dos vetores a e b. Assume-se
// que ambos a e b sejam vetores de tamanho size.
void avaliar(double* a, double* b, int size, double prod_escalar) {
    double expected = 0;
    for (int i = 0; i < size; ++i)
        expected += a[i] * b[i];
    if (expected != prod_escalar) {
        printf("Ops! recebi %f, mas esperava %f como produto escalar\n", prod_escalar, expected);
    } else {
        printf("OK! produto escalar = %f\n", prod_escalar);
    }
}
