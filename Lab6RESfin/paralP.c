#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

long double func(long double x)
{
    if (x >= 0.0 && x <= 0.25)
        return expl(sinl(x));
    else
        return expl(x) - (1/sqrtl(x));
}

typedef struct {
    int start;
    int end;
    long double h;
    long double* sum;
} thread_data;

void* thread_func(void* arg)
{
    thread_data* data = (thread_data*)arg;
    long double sum = 0;

    for (int i = data->start; i < data->end; i++)
    {
        long double x = (i * data->h + (data->h / 2.0));
        long double f = func(x);
        sum += f;
    }

    *data->sum = sum;
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[4];
    thread_data data[4];
    double e = 1e-15, del = 0;
    int n = 4;

    long double sum = 0, sum2 = 1, h = 2.0 / n;

    clock_t start = clock(); // начало измерения времени

    do
    {
        for (int i = 0; i < 4; i++)
        {
            data[i].start = i * n / 4;
            data[i].end = (i + 1) * n / 4;
            data[i].h = h;
            data[i].sum = &sum;

            pthread_create(&threads[i], NULL, thread_func, &data[i]);
        }

        for (int i = 0; i < 4; i++)
        {
            pthread_join(threads[i], NULL);
        }

        sum = sum * h;
        del = fabsl(sum2 - sum) / 3.0;
        sum2 = sum;
        n = 2 * n;
        h = h * 0.5;

    } while (del > e);

    clock_t end = clock(); // конец измерения времени
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Сумма -> %.20Lf\n", sum);
    printf("Время выполнения -> %f сек\n", time_spent);

    return 0;
}
