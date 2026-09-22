#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FIFO "fifo_ex2"

int main (void)
{
    int fd;
    pid_t pid1, pid2;
    char buffer[256];
    ssize_t n;

    if (access(FIFO, F_OK) == -1) {
        if (mkfifo(FIFO, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) != 0) {
            fprintf(stderr, "Erro ao criar FIFO %s\n", FIFO);
            exit(-1);
        }
    }

    fd = open(FIFO, O_RDWR);
    if (fd < 0) {
        perror("Erro ao abrir a FIFO");
        exit(-2);
    }

    pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(-3);
    }
    if (pid1 == 0) {
        char msg[100];
        int fdEscrita = open(FIFO, O_WRONLY);
        snprintf(msg, sizeof msg, "Mensagem do filho 1 (pid %d)\n", getpid());
        write(fdEscrita, msg, strlen(msg));
        close(fdEscrita);
        exit(0);
    }

    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(-3);
    }
    if (pid2 == 0) {
        char msg[100];
        int fdEscrita = open(FIFO, O_WRONLY);
        snprintf(msg, sizeof msg, "Mensagem do filho 2 (pid %d)\n", getpid());
        write(fdEscrita, msg, strlen(msg));
        close(fdEscrita);
        exit(0);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    n = read(fd, buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';
    printf("Pai: leu %zd bytes da FIFO:\n%s", n, buffer);

    close(fd);

    return 0;
}

