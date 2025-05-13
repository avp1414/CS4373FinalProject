#ifndef TSP_H
#define TSP_H

#include "utils.h"

graph_t read_adj_matrix(int* matrix, int n);
int* TSP(graph_t G, int n_threads);
void* TSP_search_wrapper(void* gd);
void TSP_search(glob_dat_t* gd);

#endif