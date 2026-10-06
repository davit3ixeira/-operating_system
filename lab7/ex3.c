#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define MAXFILA 8
#define TOTAL 64

static int fila[MAXFILA];
static int entra = 0;
static int sai = 0;
static int tamanho = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER;
static pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER;

void *produtor(void *arg);
void *consumidor(void *arg);

int main (void)
{
    pthread_t tProdutor, tConsumidor;

    pthread_create(&tProdutor, NULL, produtor, NULL);
    pthread_create(&tConsumidor, NULL, consumidor, NULL);

    pthread_join(tProdutor, NULL);
    pthread_join(tConsumidor, NULL);

    printf("Fim: %d itens produzidos e consumidos, fila final com %d elemento(s).\n", TOTAL, tamanho);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&can_produce);
    pthread_cond_destroy(&can_consume);

    return 0;
}

void *produtor(void *arg)
{
    unsigned int seed = (unsigned int) time(NULL);

    for (int i = 1; i <= TOTAL; i++) {
        int valor = rand_r(&seed) % 100 + 1;

        pthread_mutex_lock(&mutex);
        while (tamanho == MAXFILA) {
            printf("Produtor: fila cheia (%d/%d), esperando...\n", tamanho, MAXFILA);
            fflush(stdout);
            pthread_cond_wait(&can_produce, &mutex);
        }

        fila[entra] = valor;
        entra = (entra + 1) % MAXFILA;
        tamanho++;
        printf("Produtor: produziu %3d (item %2d) | fila: %d/%d\n", valor, i, tamanho, MAXFILA);
        fflush(stdout);

        pthread_cond_signal(&can_consume);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }

    return NULL;
}

void *consumidor(void *arg)
{
    for (int i = 1; i <= TOTAL; i++) {
        pthread_mutex_lock(&mutex);
        while (tamanho == 0) {
            printf("Consumidor: fila vazia, esperando...\n");
            fflush(stdout);
            pthread_cond_wait(&can_consume, &mutex);
        }

        int valor = fila[sai];
        sai = (sai + 1) % MAXFILA;
        tamanho--;
        printf("Consumidor: consumiu %3d (item %2d) | fila: %d/%d\n", valor, i, tamanho, MAXFILA);
        fflush(stdout);

        pthread_cond_signal(&can_produce);
        pthread_mutex_unlock(&mutex);

        if (i < TOTAL)
            sleep(2);
    }

    return NULL;
}
