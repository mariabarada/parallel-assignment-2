

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    int m = 1000, n = 1000, p = 1000;
    double *A = (double*) calloc(m*p, sizeof(double));
    double *B = (double*) calloc(p*n, sizeof(double));
    double *C = (double*) calloc(m*n, sizeof(double));

    int i, j, k;
    double start_time, end_time;
    start_time = omp_get_wtime();

    // Initialize matrices A and B
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            A[i*p+j] = i + j;
        }
    }

    for (i = 0; i < p; i++) {
        for (j = 0; j < n; j++) {
            B[i*n+j] = i * j;
        }
    }
    // Perform matrix multiplication
    #pragma omp parallel for  private(i,j,k) collapse(2) reduction(+:C[:m*n])
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < p; k++) {
                C[i*n + j] += A[i*p + k] * B[k*n + j];
            }
        }
    }

    end_time = omp_get_wtime();
    printf("Execution time: %f seconds\n", end_time - start_time);

    // Print the resulting matrix C
   // printf("Matrix C:\n");
    //for (i = 0; i < m; i++) {
      //  for (j = 0; j < n; j++) {
        //    printf("%f ", C[i*n+j]);
        //}
        //printf("\n");
    //}

    free(A);
    free(B);
    free(C);
    return 0;
}
