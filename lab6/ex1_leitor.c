#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO "fifo_ex1"

int main (void)
{
    char buffer[256];
    int fd;
    ssize_t n;

    if (access(FIFO, F_OK) == -1) {
        if (mkfifo(FIFO, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) != 0) {
            fprintf(stderr, "Erro ao criar FIFO %s\n", FIFO);
            exit(-1);
        }
    }

    printf("Leitor (pid %d): aguardando escritores na FIFO %s...\n", getpid(), FIFO);
    fflush(stdout);

    for (;;) {
        fd = open(FIFO, O_RDONLY);
        if (fd < 0) {
            perror("Erro ao abrir a FIFO para leitura");
            exit(-2);
        }

        while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[n] = '\0';
            fputs(buffer, stdout);
            fflush(stdout);
        }

        close(fd);
    }

    return 0;
}

