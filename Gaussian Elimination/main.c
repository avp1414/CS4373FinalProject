#include <stdio.h>
#include <stdlib.h>

#include "Gaussian_Elimination.h"
#include "timer.h"

int main(int argc, char** argv){
    char* f_name;
    int n;
    if(argc == 1){
        fprintf(stderr, "Usage: %s [INPUT FILE] N\n", *argv);
        return 1;
    }else if(argc == 2){
        sscanf(argv[1], "%d", &n);
        f_name = (char*) malloc(32*sizeof(char));
        sprintf(f_name, "m%04dx%04d.bin", n, n);
    }else if (argc == 3){
        f_name = argv[1];
        sscanf(argv[2], "%d", &n);
    }else{
        fprintf(stderr, "Usage: %s [INPUT FILE] N\n", *argv);
        return 1;
    }
    
    FILE* file = fopen(f_name, "rb");
    if(file == NULL){
        fprintf(stderr, "Failed to open file\n");
        return 1;
    }

    size_t n_elem = ((size_t) n)*n;
    double* matrix = malloc(n_elem*sizeof(double));

    size_t n_read = fread(matrix, sizeof(double), n_elem, file);
    if(n_read < n_elem){
        fprintf(stderr, "Expected input file with %ld doubles\n", n_elem);
        return 1;
    }

    double start,end;
    GET_TIME(start);

    triangularize(matrix, n);
    double det = getDeterminant(matrix, n);
    double log_abs_det = getLogAbsDeterminant(matrix, n);

    GET_TIME(end);

    printf("det=%le log(abs(det))=%le\nTime (seconds):\n%.3e\n", det, log_abs_det, end-start);

    return 0;
}