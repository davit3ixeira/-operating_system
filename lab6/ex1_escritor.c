#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO "fifo_ex1"

int main (void)
{
    char linha[256];
    int fd;

    if (access(FIFO, F_OK) == -1) {
        if (mkfifo(FIFO, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) != 0) {
            fprintf(stderr, "Erro ao criar FIFO %s\n", FIFO);
            exit(-1);
        }
    }

    printf("Escritor (pid %d): digite linhas de texto (Ctrl-D para encerrar):\n", getpid());
    fflush(stdout);

    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        fd = open(FIFO, O_WRONLY);
        if (fd < 0) {
            perror("Erro ao abrir a FIFO para escrita");
            exit(-2);
        }
        write(fd, linha, strlen(linha));
        close(fd);
    }

    printf("Escritor: fim da entrada padrao (Ctrl-D recebido).\n");

    return 0;
}

