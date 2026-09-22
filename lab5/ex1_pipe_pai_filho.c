#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (void)
{
    int fd[2];
    pid_t pid;
    char buffer[100];
    int n;

    if (pipe(fd) < 0) {
        puts("Erro ao abrir os pipes");
        exit(-1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(-1);
    }

    if (pid == 0) {
        close(fd[0]);

        const char *mensagem = "Ola pai, esta mensagem foi escrita pelo filho!";
        int nEscritos = write(fd[1], mensagem, strlen(mensagem) + 1);
        printf("Filho (pid %d) escreveu %d bytes no pipe.\n", getpid(), nEscritos);

        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);

        n = read(fd[0], buffer, sizeof buffer);
        printf("Pai (pid %d) leu %d bytes do pipe: \"%s\"\n", getpid(), n, buffer);

        close(fd[0]);
        wait(NULL);
    }

    return 0;
}
