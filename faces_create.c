#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"topo_strt.h"

#define ZEROTH 0

void 
faces_create(edge_ptr **dcel, int* num_faces, edge_ptr** faces) {
    
    // 0th Face Created
    *faces = (edge_ptr*) realloc(*faces, sizeof(edge_ptr) * (*num_faces + 1));
    assert(*faces);

    (*faces)[*num_faces] = (*dcel)[ZEROTH];
    (*num_faces)++;
    
}