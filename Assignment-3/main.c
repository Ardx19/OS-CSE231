#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

clock_t start, end;

int i, j, k, n, m, p, idx;
int **A, **B, **C;

pthread_mutex_t mutex;
pthread_mutex_t mutex1;

void matrixMultiplier() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void threadEnd(pthread_t th[], int num_threads) {
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            printf("Failed to join thread\n");
            return;
        }
    }
}

void* multiplier1(void* args) {
    int tempK = *(int*)args;
    pthread_mutex_lock(&mutex1);
    C[i][j] += A[i][tempK] * B[tempK][j];
    pthread_mutex_unlock(&mutex1);
    free(args);
    return NULL;
}

void* multiplier(void* args) {
    int tempK = *(int*)args;
    pthread_mutex_lock(&mutex);
    C[i][j] += A[i][tempK] * B[tempK][j];
    pthread_mutex_unlock(&mutex);
    free(args);
    return NULL;
}

void matrixProd() {
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

int main() {
    double time1, time2, time3;

    FILE *file = fopen("matrix_test_cases.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open input file.\n");
        return 1;
    }

    fscanf(file, "%d%d", &m, &n);
    fscanf(file, "%d%d", &n, &p);

    A = (int**)malloc(m * sizeof(int*));
    B = (int**)malloc(n * sizeof(int*));
    C = (int**)malloc(m * sizeof(int*));
    for (int x = 0; x < m; x++) A[x] = (int*)malloc(n * sizeof(int));
    for (int x = 0; x < n; x++) B[x] = (int*)malloc(p * sizeof(int));
    for (int x = 0; x < m; x++) C[x] = (int*)malloc(p * sizeof(int));

    for (int x = 0; x < m; x++) {
        for (int y = 0; y < n; y++) {
            fscanf(file, "%d", &A[x][y]);
        }
    }

    for (int x = 0; x < n; x++) {
        for (int y = 0; y < p; y++) {
            fscanf(file, "%d", &B[x][y]);
        }
    }

    fclose(file);

    start = clock();
    matrixMultiplier();
    end = clock();
    time1 = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    matrixProd1();
    end = clock();
    time2 = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    matrixProd();
    end = clock();
    time3 = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time taken for sequential multiplication: %f\n", time1);
    printf("Time taken for Part 1: %f\n", time2);
    printf("Time taken for Part 2: %f\n", time3);
    printf("Speedup of Part 1 over sequential: %f\n", time1 / time2);
    printf("Speedup of Part 2 over sequential: %f\n", time1 / time3);
    printf("Speedup of Part 2 over Part 1: %f\n", time2 / time3);

    for (int x = 0; x < m; x++) free(A[x]);
    for (int x = 0; x < n; x++) free(B[x]);
    for (int x = 0; x < m; x++) free(C[x]);
    free(A);
    free(B);
    free(C);

    return 0;
}
