#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TROCAS 10

void filho(int numero);

int main (void)
{
    pid_t pid1, pid2;
    int ativo;

    pid1 = fork();

    if (pid1 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid1 == 0) {
        filho(1);
        exit(0);
    }

    pid2 = fork();

    if (pid2 < 0) {
        perror("fork");
        exit(1);
    }

    if (pid2 == 0) {
        filho(2);
        exit(0);
    }

    printf("Pai: filho1 (pid %d) e filho2 (pid %d) criados\n", pid1, pid2);
    fflush(stdout);

    sleep(1);
    kill(pid2, SIGSTOP);
    ativo = 1;

    for (int troca = 1; troca <= TROCAS; troca++) {
        printf("Pai: troca %d -> ativo = filho%d (pid %d)\n",
               troca, ativo, (ativo == 1) ? pid1 : pid2);
        fflush(stdout);

        sleep(1);

        if (troca < TROCAS) {
            if (ativo == 1) {
                kill(pid1, SIGSTOP);
                kill(pid2, SIGCONT);
                ativo = 2;
            } else {
                kill(pid2, SIGSTOP);
                kill(pid1, SIGCONT);
                ativo = 1;
            }
        }
    }

    printf("Pai: %d trocas de contexto realizadas. Matando os filhos.\n", TROCAS);
    fflush(stdout);

    kill(pid1, SIGCONT);
    kill(pid2, SIGCONT);
    kill(pid1, SIGKILL);
    kill(pid2, SIGKILL);

    wait(NULL);
    wait(NULL);

    printf("Pai: filhos finalizados. Encerrando.\n");

    return 0;
}

void filho(int numero)
{
    long contador = 0;

    for (;;) {
        contador++;
        if (contador % 200000000L == 0) {
            printf("Filho %d (pid %d) esta em execucao... contador=%ld\n",
                   numero, getpid(), contador);
            fflush(stdout);
        }
    }
}
