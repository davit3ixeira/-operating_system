#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define EVER ;;

void killHandler(int sinal);

int main (void)
{
    void (*p)(int);

    p = signal(SIGKILL, killHandler);

    if (p == SIG_ERR) {
        printf("Nao foi possivel instalar o handler para SIGKILL (SIG_ERR)\n");
    } else {
        printf("signal(SIGKILL, killHandler) retornou %p, handler aparentemente instalado\n", p);
    }

    printf("PID %d aguardando sinais...\n", getpid());
    fflush(stdout);

    for (EVER) {
        pause();
    }
}

void killHandler(int sinal)
{
    printf("SIGKILL interceptado! Isso nunca deveria aparecer.\n");
    fflush(stdout);
}
