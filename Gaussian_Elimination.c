#include <math.h>
#include <stdlib.h>

#include <omp.h>

#include "Gaussian_Elimination.h"

/*
Use Gaussian elimination to triangularize square matrix while preserving determinant
*/
void triangularize(double* matrix, int n){
    double* factors = malloc(n*sizeof(double));
    for(int i = 0; i < n-1; i++){
        double piv = matrix[i*n+i];

        #pragma omp parallel for
        for(int j = i+1; j < n; j++){
            factors[j] = matrix[j*n+i]/piv;
        }

        #pragma omp parallel for collapse(2)
        for(int j = i+1; j < n; j++){
            for(int k = i; k < n; k++){
                matrix[j*n+k] -= factors[j]*matrix[i*n+k];
            }
        }
    }
    free(factors);
}

/*
Calculates determinant of a triangular matrix
*/
double getDeterminant(double* matrix, int n){
    double det = 1;
    for(int i = 0; i < n; i++){
        det *= matrix[i*n+i];
    }
    return det;
}

/*
Calculates log_10 of absolute value of determinant of a triangular matrix
*/
double getLogAbsDeterminant(double* matrix, int n){
    double det = 0;
    for(int i = 0; i < n; i++){
        det += log10(fabs(matrix[i*n+i]));
    }
    return det;
}