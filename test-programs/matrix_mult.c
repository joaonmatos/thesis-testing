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
    double **rows = (double **)calloc(side, sizeof(double *));

    for (size_t row = 0; row < side; row++)
    {
        double *r = (double *)calloc(side, sizeof(double));
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
    srand(time(NULL));

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
    matrix *m1 = matrix_new(8);
    matrix *m2 = matrix_new(8);
    matrix *out = matrix_new(8);

    matrix_fill_rand(m1);
    matrix_fill_rand(m2);

    clock_t start = clock();
    matrix_mult(m1, m2, out);
    clock_t end = clock();
    printf("matrix mult took %ld clocks to complete\n", end - start);

    start = clock();
    double trace = matrix_trace(out);
    end = clock();
    printf("matrix trace took %ld clocks to complete\n", end - start);

    matrix_destroy(m1);
    matrix_destroy(m2);
    matrix_destroy(out);

    printf("trace: %f\n", trace);
    return EXIT_SUCCESS;
}
