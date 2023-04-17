#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 4
#define MAX_SIZE 100

int matrix[MAX_SIZE][MAX_SIZE];
int size;
int pivot[MAX_THREADS];
pthread_t threads[MAX_THREADS];

void* gaussian_elimination(void* arg)
{
    int thread_id = *(int*)arg;
    int i, j, k;
    int start = pivot[thread_id];
    int end = (thread_id == MAX_THREADS-1) ? size : pivot[thread_id+1];

    for (i = start; i < end; i++) {
        for (j = i + 1; j < size; j++) {
            int factor = matrix[j][i] / matrix[i][i];
            for (k = i; k < size; k++) {
                matrix[j][k] -= factor * matrix[i][k];
            }
        }
    }

    pthread_exit(NULL);
}

int main()
{
    int i, j, k;
    int det = 1;
    int thread_id[MAX_THREADS];

    srand(time(NULL));

    // Ввод порядка матрицы
    printf("Введите порядок матрицы: ");
    scanf("%d", &size);

    // Заполнение матрицы случайными числами от 1 до 99
    printf("Исходная матрица:\n");
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            matrix[i][j] = rand() % 99 + 1;
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // Разбиваем матрицу на блоки для каждого потока
    for (i = 0; i < MAX_THREADS; i++) {
        pivot[i] = i * size / MAX_THREADS;
        thread_id[i] = i;
    }

    // Создаем потоки
    for (i = 0; i < MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, gaussian_elimination, &thread_id[i]);
    }

    // Ждем завершения потоков
    for (i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Вычисляем определитель матрицы
    for (i = 0; i < size; i++) {
        det *= matrix[i][i];
    }

    printf("Определитель: %d\n", det);

    return 0;
}
