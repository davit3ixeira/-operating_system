#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf ambiente;

void fpeHandler(int sinal);

int main (void)
{
    int a, b;

    signal(SIGFPE, fpeHandler);

    printf("Digite o primeiro numero: ");
    scanf("%d", &a);
    printf("Digite o segundo numero: ");
    scanf("%d", &b);

    if (setjmp(ambiente) == 0) {
        printf("%d + %d = %d\n", a, b, a + b);
        printf("%d - %d = %d\n", a, b, a - b);
        printf("%d * %d = %d\n", a, b, a * b);
        printf("%d / %d = %d\n", a, b, a / b);
        printf("Todas as operacoes foram calculadas com sucesso.\n");
    } else {
        printf("Nao foi possivel calcular a divisao: o handler capturou o SIGFPE.\n");
    }

    return 0;
}

void fpeHandler(int sinal)
{
    printf("Sinal SIGFPE (%d) recebido: divisao por zero detectada!\n", sinal);
    fflush(stdout);
    longjmp(ambiente, 1);
}
