#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

int trucks, bufferSize, storage_managers, max_delivery = 5, max_manager = 5;
int *arr; // Buffer
int currSize = 0;
int head = 0;
int tail = 0;
int simulation = 1;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void* delivery(void* args) {
    int id = *(int*)args + 1;
    free(args);
    while (simulation) {
        int products = rand() % max_delivery + 1;

        sem_wait(&empty); 
        pthread_mutex_lock(&mutex);

        if (!simulation) { 
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            break;
        }

        if (currSize == bufferSize) {
            printf("Truck %d failed to deliver %d products, Buffer full\n", id, products);
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            sleep(rand() % 3 + 1); 
            continue;  
        }

        int delivered = 0;
        for (int i = 0; i < products && currSize < bufferSize; i++) {
            arr[tail] = 1; 
            tail = (tail + 1) % bufferSize;
            currSize++;
            delivered++;
        }

        printf("Truck %d delivered %d products. Buffer count: %d\n", id, delivered, currSize);
        pthread_mutex_unlock(&mutex);
        sem_post(&full); 
        sleep(rand() % 3 + 1); 
    }
    return NULL;
}

void* manager(void* args) {
    int id = *(int*)args + 1;
    free(args);
    while (simulation) {
        int products = rand() % max_manager + 1;

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (!simulation) { 
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            break;
        }

        if (currSize == 0) {
            printf("Manager %d failed to manage %d products, Buffer empty\n", id, products);
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            sleep(rand() % 3 + 1); 
            continue;
        }

        int managed = 0;
        for (int i = 0; i < products && currSize > 0; i++) {
            arr[head] = 0;
            head = (head + 1) % bufferSize;
            currSize--;
            managed++;
        }

        printf("Manager %d managed %d products. Buffer count: %d\n", id, managed, currSize);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); 
        sleep(rand() % 3 + 1);
    }
    return NULL;
}


void input() {
    printf("Enter buffer size: ");
    scanf("%d", &bufferSize);
    printf("Enter number of delivery trucks: ");
    scanf("%d", &trucks);
    printf("Enter maximum deliveries: ");
    scanf("%d", &max_delivery);
    printf("Enter number of storage managers: ");
    scanf("%d", &storage_managers);
    printf("Enter maximum products managed: ");
    scanf("%d", &max_manager);
}

int main() {
    input();

    pthread_t tth[trucks];
    pthread_t smth[storage_managers];

    arr = (int*)malloc(bufferSize * sizeof(int));

    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, bufferSize); 
    sem_init(&full, 0, 0);

    for (int i = 0; i < trucks; i++) {
        int* a = (int*)malloc(sizeof(int));
        *a = i;
        pthread_create(&tth[i], NULL, &delivery, a);
    }
    for (int i = 0; i < storage_managers; i++) {
        int* b = (int*)malloc(sizeof(int));
        *b = i;
        pthread_create(&smth[i], NULL, &manager, b);
    }

    sleep(20); 
    simulation = 0; 

    
    for (int i = 0; i < bufferSize; i++) {
        sem_post(&empty);
        sem_post(&full);
    }

    for (int i = 0; i < trucks; i++) {
        pthread_join(tth[i], NULL);
    }
    for (int i = 0; i < storage_managers; i++) {
        pthread_join(smth[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(arr);

    printf("Final state of warehouse: %d products in buffer\n", currSize);
    return 0;
}
