#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define EVER ;;

void intHandler(int sinal);
void quitHandler(int sinal);

int main (void)
{
    puts("Tratamento de sinais removido. Ctrl-C e Ctrl-\\ terao a acao default.");

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
