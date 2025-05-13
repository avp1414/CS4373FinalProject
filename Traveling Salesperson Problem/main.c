#include <stdio.h>
#include <stdlib.h>

#include "TSP.h"
#include "utils.h"
#include "timer.h"

int main(int argc, char** argv){
    if(argc < 3){
        printf("Usage: %s n_threads input_file\n", *argv);
        return -1;
    }
    int n_threads;
    sscanf(argv[1], "%d", &n_threads);

    int n = 1;
    FILE* f = fopen(argv[2], "r");
    int c;
    while((c = fgetc(f)) >= 0){
        if(c == ((int) ',')){
            n++;
        }else if(c == ((int) '\n')){
            break;
        }
    }
    fclose(f);

    int* adj_matrix = malloc(n*n*sizeof(int));

    f = fopen(argv[2], "r");
    int entry;
    int idx = 0;
    while(fscanf(f, "%d%*[,]", &entry) > 0 && idx < n*n){
        adj_matrix[idx] = entry;
        idx++;
    }
    fclose(f);

    graph_t G = read_adj_matrix(adj_matrix, n);

    double start, end;
    GET_TIME(start);

    int* best_path = TSP(G, n_threads);

    GET_TIME(end);

    int len = 0;
    
    unsigned char* visited = calloc(sizeof(unsigned char), (n>>3)+1);
    for(int i = 0; i < n; i++){
        if(in_bit_field(visited, best_path[i])){
            printf("%d\n", i);
        }
        set_bit(visited, best_path[i], 1);
        len += adj_matrix[best_path[i]*n+best_path[(i+1)%n]];
    }
    printf("1:\t%d\n2:\t%d\n3:\t%f\n4:\t", len, n, end-start);
    for(int i = 0; i < n; i++){
        printf("%d", best_path[i]);
        if(i < n-1){
            printf(",");
        }
    }
    printf("\n\n");
}