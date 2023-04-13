

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXN 100 
#define MAX_THREADS 10 

int N; 
int A[MAXN][MAXN];
double det = 1.0;
int thread_count = 1;
pthread_barrier_t barrier; 

void* compute_det(void* arg) {
    int tid = *(int*)arg;
    int start_row = (N / thread_count) * tid;
    int end_row = tid == thread_count - 1 ? N : (N / thread_count) * (tid + 1);

    for (int k = 0; k < N - 1; k++) {
        for (int i = start_row + k + 1; i < end_row; i += thread_count) {
            double coef = A[i][k] / (double)A[k][k];
            for (int j = k; j < N; j++) {
                A[i][j] -= coef * A[k][j];
            }
        }
        pthread_barrier_wait(&barrier);
    }

    for (int i = start_row; i < end_row; i += thread_count) {
        det *= A[i][i];
    }
    pthread_barrier_wait(&barrier);

    pthread_exit(NULL);
}

int main() {
    printf("Введите порядок матрицы: ");
    scanf("%d", &N);
    printf("Введите элементы матрицы:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Введите количество потоков: ");
    scanf("%d", &thread_count);
    thread_count = thread_count < MAX_THREADS ? thread_count : MAX_THREADS;

    pthread_t threads[MAX_THREADS];
    int thread_ids[MAX_THREADS];
    pthread_barrier_init(&barrier, NULL, thread_count);
    for (int i = 0; i < thread_count; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, compute_det, &thread_ids[i]);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Determinant = %lf\n", det);

    pthread_barrier_destroy(&barrier);

    return 0;
}
