#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>
#include <semaphore.h>

typedef struct graph{
    int n;
    int* adj_matrix;
    int* weights;
    int* ids;
} graph_t;

typedef struct global_data{
    graph_t G;
    pthread_mutex_t mut;
    pthread_cond_t wake;
    int lower_bound;
    volatile int best_len;
    volatile int* best_path;
    volatile int term;
} glob_dat_t;

void set_bit(unsigned char* bit_field, int idx, char val);
char in_bit_field(unsigned char* bit_field, int idx);
void zip_sort(int* sort_by, int* sort_with, int n);
#endif