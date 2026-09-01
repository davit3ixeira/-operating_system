#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void childhandler(int signo);
int delay;
pid_t pid;

int main (int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Uso: %s tempo programafilho arg1 arg2 ... argn\n", argv[0]);
        exit(-1);
    }

    signal(SIGCHLD, childhandler);

    if ((pid = fork()) < 0) {
        fprintf(stderr, "Erro ao criar filho\n");
        exit(-1);
    }

    if (pid == 0) {
        execv(argv[2], &argv[2]);
        perror("execv");
        exit(-1);
    } else {
        sscanf(argv[1], "%d", &delay);
        sleep(delay);
        printf("Program %s exceeded limit of %d seconds!\n", argv[2], delay);
        kill(pid, SIGKILL);
        sleep(1);
    }

    return 0;
}

void childhandler(int signo)
{
    int status;
    pid_t filho = wait(&status);
    printf("Child %d terminated within %d seconds com estado %d.\n", filho, delay, status);
    exit(0);
}
