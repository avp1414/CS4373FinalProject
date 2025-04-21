#include <math.h>
#include "Gaussian_Elimination.h"

/*
Use Gaussian elimination to triangularize square matrix while preserving determinant
*/
void triangularize(double* matrix, int n){
    for(int i = 0; i < n-1; i++){
        double piv = matrix[i*n+i];
        for(int j = i+1; j < n; j++){
            double factor = matrix[j*n+i]/piv;
            for(int k = i; k < n; k++){ // Parallelize this loop
                matrix[j*n+k] -= factor*matrix[i*n+k];
            }
        }
    }
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
Calculates log_10 of abs of determinant of a triangular matrix
*/
double getLogAbsDeterminant(double* matrix, int n){
    double det = 0;
    for(int i = 0; i < n; i++){
        det += log10(fabs(matrix[i*n+i]));
    }
    return det;
}