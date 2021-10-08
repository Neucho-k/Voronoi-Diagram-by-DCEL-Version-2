#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"topo_strt.h"

#define FACE 0
#define ZEROTH 0

void 
dcel_create(int* num_vertcs, point_ptr ** vertcs, int* num_dcel, 
                edge_ptr **location) {
    int i;
    edge_ptr tmp_prev = NULL;

    for (i = 0; i < *num_vertcs; i++) {

        // Allocating memory for Doubly Connected Edge List (DCEL)
        *location = (edge_ptr*) realloc(*location, sizeof(edge_ptr) 
                        * (*num_dcel + 1));
        assert(*location);
        (*location)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
        assert((*location)[*num_dcel]);


        // Index of start vertex
        (*location)[*num_dcel]->idx_strt = i;

        // Index of end vertex
        if (i + 1 == *num_vertcs) {
            (*location)[*num_dcel]->idx_end = 0;
        }
        else {
            (*location)[*num_dcel]->idx_end = i + 1;
        }

        // Pointer to previous DCE & Pointer to next DCE
        (*location)[*num_dcel]->prev_edge = tmp_prev;
        tmp_prev = (*location)[*num_dcel];

        if (i + 1 > *num_vertcs || i == 0) {
            (*location)[*num_dcel]->next_edge = NULL;
        }
        else {
            (*location)[*num_dcel-1]->next_edge = (*location)[*num_dcel];
        }
        
        // Point to other half edge (in other directon)
        (*location)[*num_dcel]->otr_edge = NULL;

        // Index of its corresponding face
        (*location)[*num_dcel]->corr_face = FACE;
        
        // Index of the edge it forms a part of
        (*location)[*num_dcel]->idx_edge = *num_dcel;


        (*num_dcel)++;
    }

    (*location)[ZEROTH]->prev_edge = (*location)[*num_dcel-1];
    (*location)[*num_dcel-1]->next_edge = (*location)[ZEROTH];

    printf("DCEL CREATE Done!\n");
}