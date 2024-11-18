#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>

#define THREAD_NUM 5
sem_t channels[THREAD_NUM];

void* network(void* args){
    int i= *(int*)args;
    int semVal1,semVal2;
    sem_getvalue(&channels[i%THREAD_NUM],&semVal1);
    sem_getvalue(&channels[(i+1)%THREAD_NUM],&semVal2);
    if(semVal1==0 || semVal2==0){
        printf("Server %d is waiting...\n",i+1);
    }
    sem_wait(&channels[i%THREAD_NUM]);
    sem_wait(&channels[(i+1)%THREAD_NUM]);
    printf("Server %d is processing\n",i+1);
    sleep(1);
    sem_post(&channels[i%THREAD_NUM]);
    sem_post(&channels[(i+1)%THREAD_NUM]);
}

// SERVERS: 0    1    2    3    4
//channels:0 1  1 2  2 3  3 4  4 0

int main(){
    pthread_t servers[THREAD_NUM];
    for(int i=0;i<THREAD_NUM;i++){
        sem_init(&channels[i],0,1);
    }
    for(int j=0;j<3;j++){
        for(int i=0;i<THREAD_NUM;i++){
        int *a=(int*)malloc(sizeof(int));
        *a=i;
        if(pthread_create(&servers[i],NULL,&network,(void*)a)!=0){
            perror("Couldn't make thread\n");
            exit(0);
        }
        // sleep(1);{add this for checking if the sequence is right or if any channel is getting skipped}
        //the output should be : 
        // Server n is waiting...
        // Server n is processing
    }
    for(int i=0;i<THREAD_NUM;i++){
        if(pthread_join(servers[i],NULL)!=0){
            perror("Coudn't join thread\n");
            exit(0);
        }
    }
    }
    for(int i=0;i<THREAD_NUM;i++){
        sem_destroy(&channels[i]);
    }
}