#ifndef __ARGS_H__
#define __ARGS_H__

#include <stdio.h>


#define __PROGRAM_NAME__ "assignment-1"
#define __PROGRAM_VERSION__ "0.0.1"
#define __PROGRAM_AUTHOR__ "Vanderlei Munhoz (munhoz@proton.me)"

/**
 * @brief Definição de constantes booleanas.
*/
#define FALSE                    0
#define TRUE                     1

/**
 * @brief Configuração para uma simulação.
*/
typedef struct config {
    unsigned int sushi_chefs;
    unsigned int conveyor_belt_capacity;
    unsigned int opening_time;
    unsigned int closing_time;
    unsigned int clock_speed_multiplier;
} config_t;

extern config_t config;

/**
 * @brief Função para captura da configuração de entrada.
*/
config_t parse(int argc, char** argv);

/**
 * @brief Funções para facilitar o print de mensagens.
*/
void help();
void simulation_summary(config_t* config);
void title();
void bar();
void separator();
void usage(char* program_name);
void configuration(config_t* config);
void description();
void version();

/**
 * @brief Definições ANSI de cores para pretty-print 
*/
#define NO_COLOR    "\x1b[0m"
#define RED         "\x1b[31m"
#define GREEN       "\x1b[32m"
#define BROWN       "\x1b[33m"
#define BLUE        "\x1b[34m"
#define MAGENTA     "\x1b[35m"
#define CYAN        "\x1b[36m"
#define GRAY        "\x1b[37m"

/**
 * @brief Macro para impressão de logs em modo debug.
*/
#ifndef NDEBUG
    #define plog(...) printf(__VA_ARGS__); fflush(stdout)
#else
    #define plog(...)
#endif

/**
 * @brief Separadores.
*/
#define SEPARATOR "----------------------------------------------------------------------------------------\n"
#define BAR       "========================================================================================\n"
#define TITLE     "===================================== [SUSHI SHOP] =====================================\n"

#endif  // __ARGS_H__
