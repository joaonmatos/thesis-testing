#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float square_distance(float x1, float y1, float x2, float y2)
{
    float x_diff = (x1 - x2) * (x1 - x2);
    float y_diff = (y1 - y2) * (y1 - y2);
    float result = x_diff + y_diff;
    return result;
}

void calculate_centroid_means(size_t n_clusters, size_t gen_factor, float *centroid_xs, float *centroid_ys, size_t *centroid_counts, float *xs, float *ys, size_t *classes)
{
    // reset centroid means
    for (size_t k = 0; k < n_clusters; k++)
    {
        centroid_xs[k] = 0.0;
        centroid_ys[k] = 0.0;
        centroid_counts[k] = 0;
    }

    // update centroid means
    for (size_t i = 0; i < n_clusters * gen_factor; i++)
    {
        float x = xs[i];
        float y = ys[i];
        size_t k = classes[i];

        centroid_counts[k]++;
        centroid_xs[k] += (x - centroid_xs[k]) / centroid_counts[k];
        centroid_ys[k] += (y - centroid_ys[k]) / centroid_counts[k];
    }
}

int main()
{
    size_t n_clusters = 8;
    float cluster_seed_xs[] = {3.0, -2.1, 12.8, 0.3, -7.5, 16.2, -13.1, -0.7};
    float cluster_seed_ys[] = {3.0, 2.1, -12.8, -0.3, 7.5, 16.2, -13.1, -0.7};

    size_t allowed_switches = 2;
    size_t gen_factor = 100;
    int dist_factor = 150;

    srand(42);

    float *xs = calloc(n_clusters * gen_factor, sizeof(float));
    float *ys = calloc(n_clusters * gen_factor, sizeof(float));
    size_t *classes = calloc(n_clusters * gen_factor, sizeof(size_t));

    // randomly distribute points around seed clusters
    for (size_t k = 0; k < n_clusters; k++)
    {
        size_t base = k * gen_factor;
        float cluster_x = cluster_seed_xs[k];
        float cluster_y = cluster_seed_ys[k];
        for (size_t offset = 0; offset < gen_factor; offset++)
        {
            size_t ix = base + offset;
            float x_delta = (rand() % (dist_factor * 2) - dist_factor) / (float)100;
            xs[ix] = cluster_x + x_delta;
            float y_delta = (rand() % (dist_factor * 2) - dist_factor) / (float)100;
            ys[ix] = cluster_y + y_delta;
            classes[ix] = rand() % n_clusters;
        }
    }

    // randomly distribute classes
    for (size_t i = 0; i < n_clusters * gen_factor; i++)
    {
        int class = rand() % n_clusters;
        classes[i] = class;
    }

    float centroid_xs[n_clusters];
    float centroid_ys[n_clusters];
    size_t centroid_counts[n_clusters];

    size_t switched_observations = 0;
    int iterations = 0;

    clock_t start, end;
    start = clock();
    do
    {
        switched_observations = 0;
        iterations++;
        calculate_centroid_means(n_clusters, gen_factor, centroid_xs, centroid_ys, centroid_counts, xs, ys, classes);
        for (size_t i = 0; i < n_clusters * gen_factor; i++)
        {
            float x = xs[i];
            float y = ys[i];
            size_t class = classes[i];

            for (size_t k = 0; k < n_clusters; k++)
            {
                float centroid_x = centroid_xs[k];
                float centroid_y = centroid_ys[k];
                float centroid_distance = square_distance(x, y, centroid_x, centroid_y);

                float class_x = centroid_xs[class];
                float class_y = centroid_ys[class];
                float class_distance = square_distance(x, y, class_x, class_y);

                if (centroid_distance < class_distance)
                {
                    class = k;
                }

                if (class != classes[i])
                {
                    classes[i] = class;
                    switched_observations++;
                }
            }
        }
    } while (switched_observations > allowed_switches);
    end = clock();
    printf("k-means took %d iterations in %fs\n", iterations, (end - start) / (double)CLOCKS_PER_SEC);
}