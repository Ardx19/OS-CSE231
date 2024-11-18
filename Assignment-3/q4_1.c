#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

clock_t start,end;

int i,j,k,n,m,p;
int **A,**B,**C;

pthread_mutex_t mutex1;

void* multiplier1(void* args) {
    int tempK = *(int*)args;
    pthread_mutex_lock(&mutex1);
    C[i][j] += A[i][tempK] * B[tempK][j];
    pthread_mutex_unlock(&mutex1);
    free(args);
    return NULL;
}

void matrixProd1() {
    pthread_mutex_init(&mutex1, NULL);
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            C[i][j] = 0;
            pthread_t th[n];
            for (k = 0; k < n; k++) {
                int* tempK = (int*)malloc(sizeof(int));
                if (tempK == NULL) {
                    printf("Memory allocation failed for tempK\n");
                    return;
                }
                *tempK = k;
                if (pthread_create(&th[k], NULL, multiplier1, tempK) != 0) {
                    printf("Failed to create thread\n");
                    return;
                }
            }
            for (int idx = 0; idx < n; idx++) {
                if (pthread_join(th[idx], NULL) != 0) {
                    printf("Failed to join thread\n");
                    return;
                }
            }
        }
    }
    pthread_mutex_destroy(&mutex1);
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
    
    start=clock();
    matrixProd1();
    end=clock();

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
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken for part 1:%f\n",time_taken);
    
}