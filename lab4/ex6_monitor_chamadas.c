#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

time_t inicio;
int chamada_ativa = 0;

void iniciaHandler(int sinal);
void terminaHandler(int sinal);

int main (void)
{
    signal(SIGUSR1, iniciaHandler);
    signal(SIGUSR2, terminaHandler);

    printf("PID %d: monitor de chamadas telefonicas aguardando sinais...\n", getpid());
    printf("Envie SIGUSR1 para iniciar uma chamada e SIGUSR2 para encerra-la.\n");
    fflush(stdout);

    for (;;) {
        pause();
    }

    return 0;
}

void iniciaHandler(int sinal)
{
    inicio = time(NULL);
    chamada_ativa = 1;
    printf("Chamada iniciada.\n");
    fflush(stdout);
}

void terminaHandler(int sinal)
{
    if (!chamada_ativa) {
        printf("Nenhuma chamada em andamento no momento.\n");
        fflush(stdout);
        return;
    }

    time_t fim = time(NULL);
    long duracao = (long)(fim - inicio);
    double custo;

    if (duracao <= 60) {
        custo = duracao * 0.02;
    } else {
        custo = 60 * 0.02 + (duracao - 60) * 0.01;
    }

    int minutos = (int)(duracao / 60);
    int segundos = (int)(duracao % 60);

    printf("Chamada terminada. Duracao: %dm%02ds. Custo: R$%.2f\n", minutos, segundos, custo);
    fflush(stdout);

    chamada_ativa = 0;
}
