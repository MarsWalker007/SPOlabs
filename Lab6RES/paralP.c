#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 4

double func(double x)
{
  if (x >= 0.0 && x <= 0.25)
    return exp(sin(x));
  else
    return exp(x) - (1/sqrt(x));
}

typedef struct {
    int start;
    int end;
    double h;
    double result;
} thread_data_t;

void *thread_func(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    double x, f, sum = 0;

    for (int i = data->start; i < data->end; i++) {
        x = i * data->h + data->h / 2;
        f = func(x);
        sum += f;
    }

    data->result = sum;

    pthread_exit(NULL);
}

int main()
{
    int n = 4, i;
    double sum = 0, sum2 = 1, e = 1e-15, del = 0, h = 0.5 / n;

    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];

    do {
        sum = 0;

        for (i = 0; i < NUM_THREADS; i++) {
            thread_data[i].start = i * n / NUM_THREADS;
            thread_data[i].end = (i + 1) * n / NUM_THREADS;
            thread_data[i].h = h;
            pthread_create(&threads[i], NULL, thread_func, &thread_data[i]);
        }

        for (i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
            sum += thread_data[i].result;
        }

        sum *= h;

        del = fabs(sum2 - sum) / 3.0;
        sum2 = sum;

        n = 2 * n;
        h = 0.5 / n;

    } while (del > e);

    
    printf("Сумма -> %.13lf\n", sum);
    

    return 0;
}