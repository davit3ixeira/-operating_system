#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_REQ  "fifo_pedidos"
#define FIFO_RESP "fifo_respostas"

int main (int argc, char *argv[])
{
    int fdReq, fdResp;
    char buffer[256];
    ssize_t n;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s palavra\n", argv[0]);
        exit(-1);
    }

    fdReq = open(FIFO_REQ, O_WRONLY);
    if (fdReq < 0) {
        perror("Erro ao abrir FIFO de pedidos");
        exit(-2);
    }
    write(fdReq, argv[1], strlen(argv[1]));
    close(fdReq);

    fdResp = open(FIFO_RESP, O_RDONLY);
    if (fdResp < 0) {
        perror("Erro ao abrir FIFO de respostas");
        exit(-3);
    }
    n = read(fdResp, buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';
    close(fdResp);

    printf("Cliente (pid %d): \"%s\" -> %s", getpid(), argv[1], buffer);

    return 0;
}

