#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 3

int main (int argc, char *argv[])
{
    if (argc < 5) {
        fprintf(stderr, "Uso: %s rodadas programa1 programa2 programa3\n", argv[0]);
        exit(-1);
    }

    int rodadas;
    sscanf(argv[1], "%d", &rodadas);

    pid_t pids[NUM_PROC];
    int fatias[NUM_PROC] = {1, 2, 2}; /* 1s para o 1o processo, 2s para os demais */

    for (int i = 0; i < NUM_PROC; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork");
            exit(1);
        }

        if (pids[i] == 0) {
            char *args[] = { argv[i + 2], NULL };
            execv(argv[i + 2], args);
            perror("execv");
            exit(-1);
        }

        kill(pids[i], SIGSTOP); /* todo mundo comeca parado */
    }

    printf("Escalonador: filhos criados -> IO1 (pid %d), IO2 (pid %d), IO3 (pid %d)\n",
           pids[0], pids[1], pids[2]);
    fflush(stdout);

    for (int rodada = 1; rodada <= rodadas; rodada++) {
        for (int i = 0; i < NUM_PROC; i++) {
            printf("Escalonador: rodada %d -> executando processo %d (pid %d) por %d segundo(s)\n",
                   rodada, i + 1, pids[i], fatias[i]);
            fflush(stdout);

            kill(pids[i], SIGCONT);
            sleep(fatias[i]);
            kill(pids[i], SIGSTOP);
        }
    }

    printf("Escalonador: %d rodadas concluidas. Encerrando os processos filho.\n", rodadas);
    fflush(stdout);

    for (int i = 0; i < NUM_PROC; i++) {
        kill(pids[i], SIGCONT);
        kill(pids[i], SIGKILL);
        wait(NULL);
    }

    printf("Escalonador: todos os filhos finalizados.\n");

    return 0;
}
