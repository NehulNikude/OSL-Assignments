#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int shared_data = 0;
int read_count = 0;

sem_t mutex;
sem_t write_block;

void *reader(void *arg);
void *writer(void *arg);

int main()
{
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];

    sem_init(&mutex, 0, 1);
    sem_init(&write_block, 0, 1);

    for (int i = 0; i < NUM_READERS; i++)
    {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    for (int i = 0; i < NUM_WRITERS; i++)
    {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
    
    while (1)
    {
        sleep(1);
    }
    
    return 0;
}

void *reader(void *arg)
{
    int reader_id = *((int *)arg);

    while (1)
    {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1)
            sem_wait(&write_block);
        sem_post(&mutex);

        printf("Reader %d is reading data: %d\n", reader_id, shared_data);
        sleep(rand() % 2);

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0)
            sem_post(&write_block);
        sem_post(&mutex);

        sleep(rand() % 3);
    }
}

void *writer(void *arg)
{
    int writer_id = *((int *)arg);

    while (1)
    {
        sem_wait(&write_block);

        shared_data++;
        printf("Writer %d modified data to: %d\n", writer_id, shared_data);
        sleep(rand() % 2);

        sem_post(&write_block);
        
        sleep(rand() % 3);
    }
}