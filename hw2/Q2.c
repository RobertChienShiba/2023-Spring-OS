#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<assert.h>

#define NumOfThread 3
#define SortParam1 0
#define SortParam2 1
#define MergeParam 2

typedef struct {
    int size;
    int start;
    int *values;
}parameters;

void mergeSort(int arr[], int left, int right);
void merge(int arr[], int left, int mid, int right);

void initSort(parameters **params, int half, int total, char *argv[]){
     for (int i = 0; i < 2; i++){
        parameters *param = (parameters*)malloc(sizeof(parameters));
        assert(param != NULL);
        if (i % 2){
            param->size = (total - half);
            param->start = half;
        }
        else{
            param->size = half;
            param->start = 0;
        }
        param->values = (int*)calloc(param->size, sizeof(int));
        for (int j = 0; j < param->size; j++){
            param->values[j] = atoi(argv[j + param->start * i + 1]);
        }
        params[i] = param;
    }
}

void *sortRunner(void *params){
    parameters *data = (parameters*) params;
    mergeSort(data->values, 0, data->size - 1);
    pthread_exit(params);
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void *mergeRunner(void *params){
    parameters *data = (parameters*) params;
    merge(data->values, data->start, 
        (data->start + data->size) / 2 - 1, (data->size) - 1);
    pthread_exit(params);
}

void initMerge(parameters **data){
    parameters *param = (parameters*)malloc(sizeof(parameters));
    param->size = data[SortParam1]->size + data[SortParam2]->size;
    param->start = 0;
    param->values = (int*)calloc(param->size, sizeof(int));
    for (int i = 0; i < 2; i++){
        for(int j = 0; j < data[i]->size; j++){
            param->values[data[i]->start + j] = data[i]->values[j];
        }
    }
    data[MergeParam] = param;
}

int main(int argc, char *argv[]){
    if (argc <= 1){
        fprintf(stderr, "No arguments entered.\n");
        return 1;
    }
    int total = argc - 1;
    int half = total / 2;
    parameters *params[NumOfThread];
    initSort(params, half, total, argv);

    pthread_t sortThread1, sortThread2, mergeThread;

    pthread_create(&sortThread1, NULL, sortRunner, params[SortParam1]);
    pthread_create(&sortThread2, NULL, sortRunner, params[SortParam2]);

    pthread_join(sortThread1, (void **) &params[SortParam1]);
    pthread_join(sortThread2, (void **) &params[SortParam2]);

    for (int i = 0; i < 2; i++){
        printf("Sort Sublist %d: ", i + 1);
        for(int j = 0; j < params[i]->size; j++){
            printf("%d ", params[i]->values[j]);
        }
        printf("\n");
    }

    initMerge(params);
    pthread_create(&mergeThread, NULL, mergeRunner, params[MergeParam]);
    pthread_join(mergeThread, (void **) &params[MergeParam]);

    printf("Merge Sorted Sublist: ");
    for(int j = 0; j < params[MergeParam]->size; j++){
        printf("%d ", params[MergeParam]->values[j]);
    }
    printf("\n");

    for (int k = 0; k < NumOfThread; k++){
        free(params[k]->values);
        free(params[k]);
        params[k] = NULL;
    }
    return 0;
}