#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define MAXFILA 8
#define TOTAL 64
#define NUM_PRODUTORES 2
#define NUM_CONSUMIDORES 2

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
    pthread_t tProdutores[NUM_PRODUTORES];
    pthread_t tConsumidores[NUM_CONSUMIDORES];
    int i;

    for (i = 0; i < NUM_PRODUTORES; i++)
        pthread_create(&tProdutores[i], NULL, produtor, (void *)(intptr_t)(i + 1));

    for (i = 0; i < NUM_CONSUMIDORES; i++)
        pthread_create(&tConsumidores[i], NULL, consumidor, (void *)(intptr_t)(i + 1));

    for (i = 0; i < NUM_PRODUTORES; i++)
        pthread_join(tProdutores[i], NULL);

    for (i = 0; i < NUM_CONSUMIDORES; i++)
        pthread_join(tConsumidores[i], NULL);

    printf("Fim: %d itens produzidos e consumidos, fila final com %d elemento(s).\n", TOTAL, tamanho);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&can_produce);
    pthread_cond_destroy(&can_consume);

    return 0;
}

void *produtor(void *arg)
{
    int id = (int)(intptr_t) arg;
    int cota = TOTAL / NUM_PRODUTORES;
    unsigned int seed = (unsigned int) time(NULL) + id;

    for (int i = 1; i <= cota; i++) {
        int valor = rand_r(&seed) % 100 + 1;

        pthread_mutex_lock(&mutex);
        while (tamanho == MAXFILA) {
            printf("Produtor %d: fila cheia (%d/%d), esperando...\n", id, tamanho, MAXFILA);
            fflush(stdout);
            pthread_cond_wait(&can_produce, &mutex);
        }

        fila[entra] = valor;
        entra = (entra + 1) % MAXFILA;
        tamanho++;
        printf("Produtor %d: produziu %3d (item %2d de %d) | fila: %d/%d\n", id, valor, i, cota, tamanho, MAXFILA);
        fflush(stdout);

        pthread_cond_signal(&can_consume);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }

    return NULL;
}

void *consumidor(void *arg)
{
    int id = (int)(intptr_t) arg;
    int cota = TOTAL / NUM_CONSUMIDORES;

    for (int i = 1; i <= cota; i++) {
        pthread_mutex_lock(&mutex);
        while (tamanho == 0) {
            printf("Consumidor %d: fila vazia, esperando...\n", id);
            fflush(stdout);
            pthread_cond_wait(&can_consume, &mutex);
        }

        int valor = fila[sai];
        sai = (sai + 1) % MAXFILA;
        tamanho--;
        printf("Consumidor %d: consumiu %3d (item %2d de %d) | fila: %d/%d\n", id, valor, i, cota, tamanho, MAXFILA);
        fflush(stdout);

        pthread_cond_signal(&can_produce);
        pthread_mutex_unlock(&mutex);

        if (i < cota)
            sleep(2);
    }

    return NULL;
}
