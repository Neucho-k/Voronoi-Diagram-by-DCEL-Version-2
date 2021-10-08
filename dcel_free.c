#include<stdio.h>
#include<stdlib.h>
#include"topo_strt.h"

void 
dcel_free(int* num, edge_ptr **dcel) {
    int i;

    for (i = 0; i < *num; i++) {
        free((*dcel)[i]);
    }

    free(*dcel);
    printf("DCEL Free Completed!\n"); 
}