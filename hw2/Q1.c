#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<assert.h>

int min = INT_MAX; 
int max = INT_MIN;
double avg = 0.0;

typedef struct {
    int size;
    int *values;
}parameters;

void *calcMin(void *param);
void *calcMax(void *param);
void *calcAvg(void *param);

void *calcMin(void *params){
    parameters *data = (parameters*) params;
    int size = data->size;
    for(int i = 0;i < size;i++)
        min = min > (data->values)[i] ? (data->values)[i]:min;
    pthread_exit(0);
}

void *calcMax(void *params){
    parameters *data = (parameters*) params;
    int size = data->size;
    for(int i = 0;i < size;i++)
        max = max < (data->values)[i] ? (data->values)[i]:max;
    pthread_exit(0);
}

void *calcAvg(void *params){
    parameters *data = (parameters*) params;
    int size = data->size;
    double sum = 0;
    for(int i = 0;i < size;i++)
        sum += (data->values)[i];
    avg = sum / size;
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    if (argc <= 1){
        fprintf(stderr, "No arguments entered.\n");
        return 1;
    }
    parameters *data = (parameters*)malloc(sizeof(parameters));
    assert(data != NULL);
    data->size = argc - 1;
    data->values = (int*)calloc(data->size, sizeof(int));
    assert(data->values != NULL);
    for(int i = 0;i < data->size;i++)
        data->values[i] = atoi(argv[i + 1]);

    pthread_t minThread, maxThread, avgThread;

    pthread_create(&minThread, NULL, calcMin, data);
    pthread_create(&maxThread, NULL, calcMax, data);
    pthread_create(&avgThread, NULL, calcAvg, data);

    pthread_join(minThread, NULL);
    pthread_join(maxThread, NULL);
    pthread_join(avgThread, NULL);

    printf("The average value is %lf\n", avg);
    printf("The minimum value is %d\n", min);
    printf("The maximum value is %d\n", max);

    free(data);
    return 0;
}