#include <stdio.h>
#include <math.h>
#include <time.h>

long double func(long double x)
{
  if (x >= 0.0 && x <= 0.25)
    return exp(sinl(x));
  else
    return expl(x) - (1/sqrtl(x));
}
int main()
{
    long double f, h, x, sum, sum2 = 1, e = 1e-10, del = 0;
	  int i, n = 4;

      clock_t start = clock();

	  do
	  {
	      for (i = 0; i < n; i++)
	      {
		        h = 0.5 / n;
		        x = (i * h + (h /0.5));
	          f = func(x);
		        sum += f;
	      }
	      sum = sum * h;
	      del = fabsl(sum2 - sum) / 3.0;
	      sum2 = sum;
	      n = 2 * n;
	      printf("Сумма -> %.20Lf\n", sum);

	  }
	  while (del > e);

        clock_t end = clock(); // конец измерения времени
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Время выполнения -> %f сек\n", time_spent);
	  return 0;
}
