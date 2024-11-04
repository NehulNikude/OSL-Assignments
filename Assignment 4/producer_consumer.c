#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void *producer(void *arg);
void *consumer(void *arg);

int main()
{
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }
    
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}

void *producer(void *arg)
{
    int producer_id = *((int *)arg);
    int item;

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        item = rand() % 100;

        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = item;
        printf("Producer %d produced item %d at position %d\n", producer_id, item, in);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);

        sleep(rand() % 2);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int consumer_id = *((int *)arg);
    int item;

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);

        item = buffer[out];
        printf("Consumer %d consumed item %d from position %d\n", consumer_id, item, out);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);

        sleep(rand() % 2);
    }
    return NULL;
}