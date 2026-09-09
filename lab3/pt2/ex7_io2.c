#include <stdio.h>
#include <unistd.h>

int main (void)
{
    long contador = 0;

    for (;;) {
        contador++;
        printf("Processo IO2 (pid %d): mensagem %ld\n", getpid(), contador);
        fflush(stdout);
        usleep(200000); /* 200 ms - simula uma operacao de E/S */
    }

    return 0;
}
