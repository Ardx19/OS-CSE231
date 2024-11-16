#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int i,j,k,n,m,p,idx;
int **A,**B,**C;

pthread_mutex_t mutex;

void threadEnd(pthread_t th[], int num_threads){
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            printf("Failed to join thread\n");
            return;
        }
    }
}

void* multiplier(void* args) {
    int tempK = *(int*)args;
    pthread_mutex_lock(&mutex);
    C[i][j] += A[i][tempK] * B[tempK][j];
    pthread_mutex_unlock(&mutex);
    free(args);
    return NULL;
}

void matrixProd(){
    pthread_mutex_init(&mutex, NULL);
    int cores = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t th[cores];
    idx = 0;

    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            C[i][j] = 0;
            for (k = 0; k < n; k++) {
                int* tempK = (int*)malloc(sizeof(int));
                if (tempK == NULL) {
                    printf("Memory allocation failed for tempK\n");
                    return;
                }
                *tempK = k;

                if (pthread_create(&th[idx], NULL, multiplier, tempK) != 0) {
                    printf("Failed to create thread\n");
                    return;
                }
                idx++;

                if (idx == cores) {
                    threadEnd(th, cores);
                    idx = 0;
                }
            }
            if (idx > 0) {
                threadEnd(th, idx);
                idx = 0;
            }
        }
    }
    pthread_mutex_destroy(&mutex);
}


int main(){
    printf("Enter size of matrix A:");
    scanf("%d%d",&m,&n);
    printf("Enter size of matrix B:");
    scanf("%d%d",&n,&p);

    int val;
    A = (int**)malloc(m * sizeof(int*));
    if (A == NULL) {
        printf("Memory allocation failed for matrix A\n");
        return 1;
    }
    for (int x = 0; x < m; x++) {
        A[x] = (int*)malloc(n * sizeof(int));
        if (A[x] == NULL) {
            printf("Memory allocation failed for row %d of matrix A\n", x);
            return 1;
        }
    }

    B = (int**)malloc(n * sizeof(int*));
    if (B == NULL) {
        printf("Memory allocation failed for matrix B\n");
        return 1;
    }
    for (int x = 0; x < n; x++) {
        B[x] = (int*)malloc(p * sizeof(int));
        if (B[x] == NULL) {
            printf("Memory allocation failed for row %d of matrix B\n", x);
            return 1;
        }
    }

    C = (int**)malloc(m * sizeof(int*));
    if (C == NULL) {
        printf("Memory allocation failed for matrix C\n");
        return 1;
    }
    for (int x = 0; x < m; x++) {
        C[x] = (int*)malloc(p * sizeof(int));
        if (C[x] == NULL) {
            printf("Memory allocation failed for row %d of matrix C\n", x);
            return 1;
        }
    }

    printf("Enter elements for matrix A (%d x %d):\n", m, n);
    for (int x = 0; x < m; x++) {
        for (int y = 0; y < n; y++) {
            printf("A[%d,%d]=",x,y);
            scanf("%d", &val);
            A[x][y] = val;
        }
    }

    printf("Enter elements for matrix B (%d x %d):\n", n, p);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < p; y++) {
            printf("B[%d,%d]=",x,y);
            scanf("%d", &val);
            B[x][y] = val;
        }
    }
    
    matrixProd();

    for(int x=0;x<m;x++){
        for(int y=0;y<p;y++){
            printf(" %d ",C[x][y]);
        }
        printf("\n");
    }

    for (int x = 0; x < m; x++){
        free(A[x]);
    }
    free(A);

    for (int x = 0; x < n; x++){
        free(B[x]);
    }
    free(B);
    
    for (int x = 0; x < m; x++){
        free(C[x]);
    }
    free(C);
    
}