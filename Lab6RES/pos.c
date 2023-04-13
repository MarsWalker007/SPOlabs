#include <stdio.h>
#include <math.h>

double func(double x)
{
  if (x >= 0.0 && x <= 0.25)
    return exp(sin(x));
  else
    return exp(x) - (1/sqrt(x));
}

int main()
{
    double f, h, x, sum = 0, sum2 = 1, e = 1e-15, del = 0;
    int i, n = 4;

    do
    {
        sum = 0;
        for (i = 0; i < n; i++)
        {
            h = 0.5 / n;
            x = (i * h + (h / 2));
            f = func(x);
            sum += f;
        }

        sum = sum * h;
        del = fabs(sum2 - sum) / 3.0;
        sum2 = sum;
        n = 2 * n;

        
    }
    while (del > e);


    printf("Сумма -> %.13lf\n", sum);
    

    return 0;
}