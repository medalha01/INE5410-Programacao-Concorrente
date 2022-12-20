#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

int main(int argc, char **argv)
{

    pid_t pid;
    int x = 0;
    pid = fork();
    if (pid != 0)
    {
        pid = fork();
    }
    if (pid == 0)
    {
        printf("Processo %d, filho de %d\n", getpid(), getppid());
        x = 1;
        while (wait(NULL) >= 0)
            ;
    }

    for (int i = 0; i < 3 && x == 1; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            x = 2;
            printf("Processo %d, filho de %d\n", getpid(), getppid());
            sleep(5);
        }
    }

    while (wait(NULL) >= 0)
        ;
    if (x == 1)
    {
        printf("Processo %d finalizado\n", getpid());
    }
    if (x == 0)
    {
        printf("Processo principal %d finalizado\n", getpid());
    }
    return 0;
}
