#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct matrix
{
    size_t side;
    double **rows;
} matrix;

matrix *matrix_new(size_t sqrt_n)
{
    size_t side = sqrt_n * sqrt_n;
    double **rows = calloc(side, sizeof(double *));

    for (size_t row = 0; row < side; row++)
    {
        double *r = calloc(side, sizeof(double));
        rows[row] = r;
    }

    matrix *out = (matrix *)malloc(sizeof(matrix));
    out->side = side;
    out->rows = rows;

    return out;
}

void matrix_destroy(matrix *m)
{
    double **rows = m->rows;

    for (size_t row = 0; row < m->side; row++)
    {
        free(rows[row]);
    }

    free(rows);
    free(m);
}

double matrix_mult_coord(matrix *a, matrix *b, size_t i, size_t j)
{
    double acc = 0.0;

    for (size_t k = 0; k < a->side; k++)
    {
        acc += a->rows[i][k] * b->rows[k][j];
    }

    return acc;
}

void matrix_mult(matrix *a, matrix *b, matrix *out)
{
    size_t side = a->side;

    for (size_t i = 0; i < side; i++)
    {
        for (size_t j = 0; j < side; j++)
        {
            out->rows[i][j] = matrix_mult_coord(a, b, i, j);
        }
    }
}

void matrix_fill_rand(matrix *m)
{
    for (size_t i = 0; i < m->side; i++)
    {
        for (size_t j = 0; j < m->side; j++)
        {
            m->rows[i][j] = ((double)rand()) / 100.0;
        }
    }
}

double matrix_trace(matrix *m)
{
    double trace = 0.0;

    for (size_t i = 0; i < m->side; i++)
    {
        trace += m->rows[i][i];
    }

    return trace;
}

int main(void)
{
    srand(42);

    matrix *m1 = matrix_new(35);
    matrix *m2 = matrix_new(35);
    matrix *out = matrix_new(35);

    matrix_fill_rand(m1);
    matrix_fill_rand(m2);

    clock_t start = clock();
    matrix_mult(m1, m2, out);
    clock_t end = clock();
    printf("%f\n", (end - start) / (double)(CLOCKS_PER_SEC / 1000));
    double trace = matrix_trace(out);
    fprintf(stderr, "%f\n", trace);

    return EXIT_SUCCESS;
}
