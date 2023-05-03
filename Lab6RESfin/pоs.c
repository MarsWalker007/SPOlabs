 
#include <stdio.h>
#include <math.h>
#include <time.h>

long double func(long double x)
{
  if (x >= 0.0 && x <= 0.25)
    return expl(sinl(x));
  else
    return expl(x) - (1/sqrtl(x));
}

void delay(int seconds) {
    clock_t start_time = clock();
    while (clock() < start_time + seconds * CLOCKS_PER_SEC);
}

int main()
{
    long double f, h, x, sum, sum2 = 1, e = 1e-15, del = 0;
    int i, n = 4;

    clock_t start = clock();
    delay(18);
    do
    {
        sum = 0;
        for (i = 0; i < n; i++)
        {
            h = 0.5 / n;
            x = (i * h + (h / 2.0));
            f = func(x);
            sum += f;
        }

        sum = sum * h;
        del = fabsl(sum2 - sum) / 3.0;
        sum2 = sum;
        n = 2 * n;
    } while (del > e);

    clock_t end = clock(); // конец измерения времени
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Сумма -> %.20Lf\n", sum);
    printf("Время выполнения -> %f сек\n", time_spent);

    return 0;
}
