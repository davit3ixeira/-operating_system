#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int main (int argc, char *argv[])
{
    int fdIn, fdOut;
    int c;

    if (argc < 3) {
        fprintf(stderr, "Uso: %s arquivo_entrada arquivo_saida\n", argv[0]);
        exit(-1);
    }

    fdIn = open(argv[1], O_RDONLY);
    if (fdIn < 0) {
        perror("Erro ao abrir arquivo de entrada");
        exit(-1);
    }

    fdOut = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdOut < 0) {
        perror("Erro ao abrir arquivo de saida");
        exit(-1);
    }

    if (dup2(fdIn, 0) < 0) {
        perror("Erro no dup2 de stdin");
        exit(-1);
    }

    if (dup2(fdOut, 1) < 0) {
        perror("Erro no dup2 de stdout");
        exit(-1);
    }

    close(fdIn);
    close(fdOut);

    while ((c = getchar()) != EOF) {
        putchar(toupper(c));
    }

    return 0;
}
