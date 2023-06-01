#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <assert.h>

#define NUM_THREADS 5
#define NUM_POINTS_PER_THREAD 1000

int total_incircle = 0, total_point = 0;
pthread_mutex_t mutex;

void *monteCarlo(void *params) {
    int i;
    double x, y, radius;
    int incircle = 0;

    for (i = 0; i < NUM_POINTS_PER_THREAD; i++) {
        x = ((double)rand() / RAND_MAX) * 2 - 1;
        y = ((double)rand() / RAND_MAX) * 2 - 1;

        radius = sqrt(x * x + y * y);
        if (radius <= 1.0) {
            incircle++;
        }
    }
    pthread_mutex_lock(&mutex);
    printf("%d points fall inside the circle\n", incircle);
    printf("Estimated value of pi per thread: %f\n", 4.0 * incircle / NUM_POINTS_PER_THREAD);
    total_incircle += incircle;
    total_point += NUM_POINTS_PER_THREAD;
    printf("Accumulate pi estimates from %d points: %f\n", total_point, 4.0 * total_incircle / total_point);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {

    srand(20000154);

    pthread_t threads[NUM_THREADS];
    int i;

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, monteCarlo, NULL);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    // assert(total_point == NUM_THREADS * NUM_POINTS_PER_THREAD);
    double pi_estimate = 4.0 * total_incircle / total_point;
    printf("Estimated value of pi: %f\n", pi_estimate);

    return 0;
}
