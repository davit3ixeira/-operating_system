#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define EVER ;;

void intHandler(int sinal);
void quitHandler(int sinal);

int main (void)
{
    void (*p)(int);

    p = signal(SIGINT, intHandler);
    printf("Endereco do manipulador anterior %p\n", p);

    p = signal(SIGQUIT, quitHandler);
    printf("Endereco do manipulador anterior %p\n", p);

    puts("Ctrl-C desabilitado. Use Ctrl-\\ para terminar");

    for (EVER) {
        pause();
    }
}

void intHandler(int sinal)
{
    printf("Voce pressionou Ctrl-C (%d)\n", sinal);
    fflush(stdout);
}

void quitHandler(int sinal)
{
    printf("Terminando o processo...\n");
    exit(0);
}
