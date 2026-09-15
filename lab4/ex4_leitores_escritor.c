#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_MSGS 10
#define TAM_MSG 64

void escritor(int fdEscrita);
void leitor(int fdLeitura, int numero);

int main (void)
{
    int fd[2];
    pid_t pidEscritor, pidLeitor1, pidLeitor2;

    if (pipe(fd) < 0) {
        puts("Erro ao abrir os pipes");
        exit(-1);
    }

    pidEscritor = fork();
    if (pidEscritor < 0) {
        perror("fork");
        exit(-1);
    }
    if (pidEscritor == 0) {
        close(fd[0]);
        escritor(fd[1]);
        close(fd[1]);
        exit(0);
    }

    pidLeitor1 = fork();
    if (pidLeitor1 < 0) {
        perror("fork");
        exit(-1);
    }
    if (pidLeitor1 == 0) {
        close(fd[1]);
        leitor(fd[0], 1);
        close(fd[0]);
        exit(0);
    }

    pidLeitor2 = fork();
    if (pidLeitor2 < 0) {
        perror("fork");
        exit(-1);
    }
    if (pidLeitor2 == 0) {
        close(fd[1]);
        leitor(fd[0], 2);
        close(fd[0]);
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(pidEscritor, NULL, 0);
    waitpid(pidLeitor1, NULL, 0);
    waitpid(pidLeitor2, NULL, 0);

    printf("Pai: escritor e os dois leitores terminaram.\n");

    return 0;
}

void escritor(int fdEscrita)
{
    char msg[TAM_MSG];

    for (int i = 1; i <= NUM_MSGS; i++) {
        snprintf(msg, sizeof msg, "mensagem %d do escritor (pid %d)", i, getpid());
        write(fdEscrita, msg, sizeof msg);
        printf("Escritor (pid %d) produziu: \"%s\"\n", getpid(), msg);
        fflush(stdout);
        usleep(500000);
    }
}

void leitor(int fdLeitura, int numero)
{
    char msg[TAM_MSG];
    int n;

    while ((n = read(fdLeitura, msg, sizeof msg)) > 0) {
        printf("Leitor %d (pid %d) consumiu: \"%s\"\n", numero, getpid(), msg);
        fflush(stdout);
        sleep(1);
    }
}
