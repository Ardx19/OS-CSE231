#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t lockA, lockB;

void *threadScheduler(void *arg){
    int id=*(int*)arg;
    printf("T%d: waiting for Lock A\n",id);
    pthread_mutex_lock(&lockA);
    printf("T%d: acquired Lock A\n",id);

    printf("T%d: waiting for Lock B\n",id);
    pthread_mutex_lock(&lockB);
    printf("T%d: acquired Lock B\n",id);
    
    printf("T%d: closing lock B\n",id);
    pthread_mutex_unlock(&lockB);
    printf("T%d: Lock B closed\n",id);

    printf("T%d: closing lock A\n",id);
    pthread_mutex_unlock(&lockA);
    printf("T%d: Lock A closed\n",id);

    return NULL;
}

int main(){
    pthread_t T1,T2,T3;
    int t1=1;
    int t2=2;
    int t3=3;
    pthread_mutex_init(&lockA, NULL);
    pthread_mutex_init(&lockB, NULL);


    for(int i=0;i<3;i++){
    pthread_create(&T1,NULL,threadScheduler,&t1);
    printf("Thread %d acquired lock %d times\n",t1,i);
    pthread_create(&T2,NULL,threadScheduler,&t2);
    printf("Thread %d acquired lock %d times\n",t2,i);
    pthread_create(&T3,NULL,threadScheduler,&t3);
    printf("Thread %d acquired lock %d times\n",t3,i);

    pthread_join(T1,NULL);
    pthread_join(T2,NULL);
    pthread_join(T3,NULL);
    }

    pthread_mutex_destroy(&lockA);
    pthread_mutex_destroy(&lockB);
    return 0;
}