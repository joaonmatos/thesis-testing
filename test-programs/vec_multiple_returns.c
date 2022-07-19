#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *slice_new(size_t size)
{
    return (int *)calloc(size, sizeof(int));
}

void slice_rand_fill(int *slice, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        slice[i] = rand();
    }
}

int *slice_ref(int *slice, size_t size, size_t offset)
{
    if (offset >= size)
    {
        return NULL;
    }
    return slice + offset;
}

size_t slice_size(int *slice, size_t size)
{
    return size;
}

int main()
{
    srand(42);

    size_t size = 1000000;
    int *slice = slice_new(size);
    slice_rand_fill(slice, size);

    unsigned int acc = 0;

    clock_t start = clock();
    for (size_t i = 0; i < slice_size(slice, size); i++)
    {
        int *elem;
        elem = slice_ref(slice, size, i);
        acc += (unsigned int)*elem;
    }
    clock_t end = clock();
    fprintf(stderr, "%u\n", acc);

    printf("%f\n", (end - start) / (double)(CLOCKS_PER_SEC / 1000));
    return EXIT_SUCCESS;
}