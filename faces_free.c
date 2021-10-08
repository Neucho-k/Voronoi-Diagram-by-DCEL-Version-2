#include<stdio.h>
#include<stdlib.h>
#include"topo_strt.h"

void
faces_free(int* num_faces, edge_ptr** faces){

    free(*faces);
    printf("Free faces done!\n");
}