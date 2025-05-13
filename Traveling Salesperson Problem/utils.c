#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void set_bit(unsigned char* bit_field, int idx, char val){
    unsigned char mask = (128>>(idx&7));
    if(val){
        bit_field[idx>>3] |= mask;
    }else{
        bit_field[idx>>3] &= ~mask;
    }
}

char in_bit_field(unsigned char* bit_field, int idx){
    return ((bit_field[idx>>3] & (128>>(idx&7))) != 0);
}

// Heapsort sort_by, sort_with follows sort_by
void zip_sort(int* sort_by, int* sort_with, int n){
    int start = n>>1;
    int end = n;
    while(end > 1){
        if(start > 0){
            start--;
        }else{
            end--;
            int bt = sort_by[0];
            int wt = sort_with[0];
            sort_by[0] = sort_by[end];
            sort_with[0] = sort_with[end];
            sort_by[end] = bt;
            sort_with[end] = wt;
        }
        int root = start;
        int child;
        while((child = (root<<1)+1) < end){
            if(child + 1 < end){
                if(sort_by[child+1] > sort_by[child]){
                    child++;
                }
            }
            if(sort_by[child] > sort_by[root]){
                int bt = sort_by[root];
                int wt = sort_with[root];
                sort_by[root] = sort_by[child];
                sort_with[root] = sort_with[child];
                sort_by[child] = bt;
                sort_with[child] = wt;
                root = child;
            }else{
                break;
            }
        }
    }
}