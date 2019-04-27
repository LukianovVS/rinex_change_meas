#include "charstring.h"
#include <cmath>

double str2float(char str[], int N, int *flag_empty)
{
    double x = 0;
    double sign = 1;
    int j = 0;
    int fp = 0;

    int counter_space = 0;


    for (int k = 0; k < N; k++)
    {
      if (fp)
        j++;

      if (str[k] == ' ')
        counter_space ++;
      else if (str[k] == '-')
        sign = -1;
      else if( str[k] == '.')
        fp = 1;
      else
        x = x*10 + (str[k] - '0');
    }

    if (flag_empty && counter_space == N)
        *flag_empty = 1;

    return sign * x / pow(10, j);
}

int str2int(char str[], int N)
{
  int x = 0;

  for (int k = 0; k < N; k++)
  {
    if (str[k] == ' ')
      continue;
    else
      x = x * 10 + (str[k] - '0');
  }

  return x;
}

