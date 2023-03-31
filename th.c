#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define m 1000
#define p 1000
#define n 1000

int *A, *B, *C;
pthread_mutex_t lock;

void *matrix_multiplication(void *arg) {
    int *tid = (int*)arg;
    int start = (*tid * m) / 16;
    int end = ((*tid) + 1) * m / 16;
    
    for(int i = start; i < end; i++) {
        for(int j = 0; j < n; j++) {
            int sum = 0;
            for(int k = 0; k < p; k++) {
                sum += A[i * p + k] * B[k * n + j];
            }
            pthread_mutex_lock(&lock);
            C[i * n + j] += sum;
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(NULL);
}

int main() {
    A = calloc(m * p, sizeof(int));
    B = calloc(p * n, sizeof(int));
    C = calloc(m * n, sizeof(int));
    
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < p; j++) {
            A[i * p + j] = i + j;
        }
    }
    
    for(int i = 0; i < p; i++) {
        for(int j = 0; j < n; j++) {
            B[i * n + j] = i * j;
        }
    }

    pthread_t threads[16];
    int thread_ids[16];
    
    clock_t start_time = clock();
    
    for(int i = 0; i < 16; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, matrix_multiplication, &thread_ids[i]);
    }
    
    for(int i = 0; i < 16; i++) {
        pthread_join(threads[i], NULL);
    }
    
    clock_t end_time = clock();
    /*
   printf("Matrix A:\n");
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < p; j++) {
            printf("%d ", A[i * p + j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix B:\n");
    for(int i = 0; i < p; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d ", B[i * n + j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix C:\n");
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d ", C[i * n + j]);
        }
        printf("\n");
    }
    */
    printf("\nExecution time: %f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
}
