#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

double func(double x) {
    if (x >= 0.0 && x <= 0.25)
        return exp(sin(x));
    else
        return exp(x) - (1 / sqrt(x));
}

struct int_args {
    int tid;
    int n;
    double h;
    double *sum;
    pthread_mutex_t *mutex;
};

void *integrate(void *arg) {
    struct int_args *args = (struct int_args *) arg;
    int n = args->n;
    double h = args->h;
    double sum = 0.0;
    for (int i = args->tid; i < n; i += 2) {
        double x = (i * h + (h / 2));
        double f = func(x);
        sum += f;
    }

    pthread_mutex_lock(args->mutex);
    *args->sum += sum;
    pthread_mutex_unlock(args->mutex);

    pthread_exit(NULL);
}

int main() {
    int n = 4;
    double h, sum = 0.0, sum2 = 1.0, e = 1e-15, del = 0.0;
    pthread_t threads[2];
    struct int_args args[2];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    do {
        sum = 0.0;

        h = 0.5 / n;

        for (int i = 0; i < 2; i++) {
            args[i].tid = i;
            args[i].n = n;
            args[i].h = h;
            args[i].sum = &sum;
            args[i].mutex = &mutex;
            pthread_create(&threads[i], NULL, integrate, (void *) &args[i]);
        }

        for (int i = 0; i < 2; i++) {
            pthread_join(threads[i], NULL);
        }

        sum *= h;

        del = fabs(sum2 - sum) / 3.0;
        sum2 = sum;
        n *= 2;
    } while (del > e);

    printf("Сумма -> %.20lf\n", sum);

    return 0;
}