#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_REQ  "fifo_pedidos"
#define FIFO_RESP "fifo_respostas"

int main (void)
{
    char buffer[256];
    int fdReq, fdResp;
    ssize_t n, total;

    if (access(FIFO_REQ, F_OK) == -1) {
        if (mkfifo(FIFO_REQ, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) != 0) {
            fprintf(stderr, "Erro ao criar FIFO %s\n", FIFO_REQ);
            exit(-1);
        }
    }
    if (access(FIFO_RESP, F_OK) == -1) {
        if (mkfifo(FIFO_RESP, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) != 0) {
            fprintf(stderr, "Erro ao criar FIFO %s\n", FIFO_RESP);
            exit(-1);
        }
    }

    printf("Servidor (pid %d): aguardando clientes...\n", getpid());
    fflush(stdout);

    for (;;) {
        fdReq = open(FIFO_REQ, O_RDONLY);
        if (fdReq < 0) {
            perror("Erro ao abrir FIFO de pedidos");
            exit(-2);
        }

        total = 0;
        while ((n = read(fdReq, buffer + total, sizeof(buffer) - 1 - total)) > 0) {
            total += n;
        }
        buffer[total] = '\0';
        close(fdReq);

        if (total == 0) {
            continue;
        }

        if (buffer[total - 1] == '\n') {
            buffer[total - 1] = '\0';
        }

        printf("Servidor: recebeu \"%s\", convertendo para maiusculas...\n", buffer);
        fflush(stdout);

        for (int i = 0; buffer[i]; i++) {
            buffer[i] = toupper((unsigned char) buffer[i]);
        }
        strcat(buffer, "\n");

        fdResp = open(FIFO_RESP, O_WRONLY);
        if (fdResp < 0) {
            perror("Erro ao abrir FIFO de respostas");
            continue;
        }
        write(fdResp, buffer, strlen(buffer));
        close(fdResp);
    }

    return 0;
}

