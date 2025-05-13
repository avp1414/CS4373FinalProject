#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include <pthread.h>

#include "TSP.h"

graph_t read_adj_matrix(int* matrix, int n){
    graph_t G;
    G.n = n;
    G.adj_matrix = matrix;
    G.weights = malloc(n*n*sizeof(int));
    G.ids = malloc(n*n*sizeof(int));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            G.weights[i*n+j] = matrix[i*n+j];
            G.ids[i*n+j] = j;
        }
        zip_sort(G.weights+i*n, G.ids+i*n, n);
    }
    return G;
}

int* TSP(graph_t G, int n_threads){
    glob_dat_t gd;
    gd.G = G;

    gd.best_path = malloc(G.n*sizeof(int));
    gd.best_len = __INT_MAX__;
    gd.lower_bound = 0;
    for(int i = 0; i < G.n; i++){
        gd.lower_bound += G.weights[i*G.n];
    }

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t finish_cond = PTHREAD_COND_INITIALIZER;
    gd.mut = mutex;
    gd.wake = finish_cond;
    gd.term = 0;

    pthread_t threads[n_threads];
    for(int i = 0; i < n_threads; i++){
        pthread_create(&threads[i], NULL, TSP_search_wrapper, &gd);
    }

    struct timespec t_wait;
    struct timeval now;
    gettimeofday(&now, NULL);
    t_wait.tv_sec = now.tv_sec+60;
    t_wait.tv_nsec = now.tv_usec*1000;
    pthread_mutex_lock(&gd.mut);
    pthread_cond_timedwait(&gd.wake, &gd.mut, &t_wait);
    pthread_mutex_unlock(&gd.mut);
    gd.term = 1;
    for(int i = 0; i < n_threads; i++){
        pthread_join(threads[i], NULL);
    }

    return (int*) gd.best_path;
}

void* TSP_search_wrapper(void* gd){
    TSP_search((glob_dat_t*) gd);
    return NULL;
}

#define K -0.75
void TSP_search(glob_dat_t* gd){
    int* path = malloc(gd->G.n*sizeof(int));
    unsigned char* visited = calloc(sizeof(unsigned char), (gd->G.n>>3)+1);
    while(1){
        int p_len = 0;
        path[0] = 0;
        set_bit(visited, 0, 1);
        for(int l = 1; l < gd->G.n; l++){
            int n_idx;
            while((n_idx = (int) (log((rand()+0.5)/(RAND_MAX+0.5))*K)) > gd->G.n);
            path[l] = gd->G.ids[path[l-1]*gd->G.n+n_idx];
            while(in_bit_field(visited, path[l])){
                n_idx = (n_idx+1)%gd->G.n;
                path[l] = gd->G.ids[path[l-1]*gd->G.n+n_idx];
            }
            set_bit(visited, path[l], 1);
            p_len += gd->G.weights[path[l-1]*gd->G.n+n_idx];
        }
        p_len += gd->G.adj_matrix[path[gd->G.n-1]*gd->G.n];
        if(p_len < gd->best_len){
            pthread_mutex_lock(&(gd->mut));
            if(p_len < gd->best_len){
                gd->best_len = p_len;
                for(int i = 0; i < gd->G.n; i++){
                    gd->best_path[i] = path[i];
                }
                if(p_len == gd->lower_bound){
                    pthread_cond_broadcast(&(gd->wake));
                }
            }
            pthread_mutex_unlock(&(gd->mut));
        }
        if(gd->term == 1){
            free(path);
            free(visited);
            return;
        }
        for(int i = 0; i < (gd->G.n>>3)+1; i++){
            visited[i] = 0;
        }
    }
}