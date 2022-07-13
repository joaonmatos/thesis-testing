#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct gaussian_point
{
    double x;
    double y;
};

typedef struct gaussian_point pt_t;

double sqrt(double n)
{
    double guess = n / 2.0;
    guess -= (guess * guess - n) / (2 * guess);
    guess -= (guess * guess - n) / (2 * guess);
    guess -= (guess * guess - n) / (2 * guess);
    guess -= (guess * guess - n) / (2 * guess);
    return guess;
}

double distance(pt_t *p1, pt_t *p2)
{
    double x_diff = p1->x - p2->x;
    double y_diff = p1->y - p2->y;
    double sum = x_diff * x_diff + y_diff * y_diff;
    return sqrt(sum);
}

int parse_double(char *str, double *out)
{
    char *endptr = NULL;
    double parsed = strtod(str, &endptr);
    if (endptr == str)
    {

        return -1;
    }
    *out = parsed;
    return 0;
}

bool calc_cond(int i)
{
    double sqr = sqrt(i);
    double sqr_m_1 = sqrt(i - 1);
    return sqr < sqr_m_1;
}

int main(void)
{
    for (int i = 1; i < 4000000; i++)
    {
        if (calc_cond(i))
        {
            printf("%d\n", i);
        }
    }
}
