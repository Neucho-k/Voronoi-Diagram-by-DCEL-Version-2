#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"watchtowers.h"
#include"topo_strt.h"
#include"calc.h"

#define NUM_NEWPT 2
#define NEW_PT 1
#define STRT_NEWPT 2
#define END_NEWPT 1
#define HALF 0.50
#define NEW_HEDGE_1 1
#define NEW_HEDGE_2 2
#define TWIN_EDGE 1
#define TRUE 1
#define FALSE 0
#define NUM_ITCT_PT 2

void strt_n_end_edge(int* idx_strt_dcel, int* idx_end_dcel, 
                        int* num_dcel, edge_ptr **dcel, itct_p_t** itct_ps);
void 
strt_end_edge_chk(int* idx_dcel, int codn, int num_dcel, edge_ptr **dcel, int nth);

void 
split_polygon(  int* num_vcells, vorocells_ptr** vorocells, 
                int* num_vertics, point_ptr **vertics, 
                int* num_dcel, edge_ptr **dcel, 
                int* num_tower, tower_ptr** towers,
                itct_p_t **itct_ps){

    int nth, idx_strt_p, idx_end_p, nth_dcel;
    int strt_dup = FALSE, end_dup = FALSE;
    int idx_strt_dcel, idx_end_dcel, nth_ncell, nth_edge = *num_dcel;
    int strt_edge_twin = FALSE, end_edge_twin = FALSE;

    edge_ptr tmp_C1A2_edge, tmp_C2C1_edge;
    edge_ptr tmp_B1C2_edge, tmp_edge, new_edge_ptr;

    // Reading the split data
    strt_n_end_edge(&idx_strt_dcel, &idx_end_dcel, num_dcel, dcel, itct_ps);
    
    /************************vertics**************************************/
    // Allocating memory for two new intersection points
    
    // First intersection point at the strarted edge
    printf("itct_ps[0].x = %f\n", (*itct_ps)[0].x);
    if (vertics_check((*itct_ps)[0], *num_vertics, vertics, &nth) == TRUE){
        printf("\nStart point duplicated!\n");
        idx_strt_p = nth;
        strt_dup = TRUE;
        strt_end_edge_chk(&idx_strt_dcel, TRUE, *num_dcel, dcel, idx_strt_p);
        (*itct_ps)[0].idx_edge = idx_strt_dcel;
    }
    else {
        printf("\nStart point duplicated not found!\n");
        *num_vertics+=NEW_PT;
        *vertics=(point_ptr*)realloc(*vertics,sizeof(point_ptr)*(*num_vertics));
        assert(*vertics);

        idx_strt_p = *num_vertics - NEW_PT;
        (*vertics)[idx_strt_p] = malloc(sizeof(point_t));
        assert((*vertics)[idx_strt_p]);

        (*vertics)[idx_strt_p]->x = (*itct_ps)[0].x;
        (*vertics)[idx_strt_p]->y = (*itct_ps)[0].y;
    }

    // Second intersection point at the ended edge
    if (vertics_check((*itct_ps)[1], *num_vertics, vertics, &nth) == TRUE){
        printf("\nEnd point duplicated!\n");
        idx_end_p = nth;
        end_dup = TRUE;
        strt_end_edge_chk(&idx_end_dcel, FALSE, *num_dcel, dcel, idx_end_p);
        (*itct_ps)[1].idx_edge = idx_end_dcel;
    }
    else {
        printf("\nEnd point duplicated not found!\n");
        *num_vertics+=NEW_PT;
        *vertics=(point_ptr*)realloc(*vertics,sizeof(point_ptr)*(*num_vertics));
        assert(*vertics);

        idx_end_p = *num_vertics - NEW_PT;
        (*vertics)[idx_end_p] = malloc(sizeof(point_t));
        assert((*vertics)[idx_end_p]);

        
        (*vertics)[idx_end_p]->x = (*itct_ps)[1].x;
        (*vertics)[idx_end_p]->y = (*itct_ps)[1].y;

    }
    printf("idx_strt_p = %d\n", idx_strt_p);
    printf("idx_strt_dcel = %d\n", idx_strt_dcel);
    printf("idx_end_p = %d\n", idx_end_p);
    printf("idx_end_dcel = %d\n", idx_end_dcel);
    /***************************dcel**************************************/
    // New edge(first-half, clockwise) created and numbered in DCEL
    
    *dcel=(edge_ptr*)realloc(*dcel,sizeof(edge_ptr)*
                                    (*num_dcel+NEW_HEDGE_2));
    assert(*dcel);
    (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
    assert((*dcel)[*num_dcel]);
    new_edge_ptr = (*dcel)[*num_dcel]; // New edge pointer saved for future used

    new_edge_ptr->idx_strt = (idx_strt_p);
    new_edge_ptr->idx_end = idx_end_p;
    new_edge_ptr->next_edge = (*dcel)[idx_end_dcel];
    new_edge_ptr->prev_edge = (*dcel)[idx_strt_dcel];
    new_edge_ptr->corr_face = (*dcel)[idx_strt_dcel]->corr_face; // Old face
    new_edge_ptr->idx_edge = nth_edge; // clockweise half edge

    (*num_dcel)++; // Number of dcel update
    nth_edge++;

    // Number of faces update
    (*num_vcells)++;
    nth_ncell = *num_vcells - 1;
    *vorocells = (vorocells_ptr*) realloc(*vorocells, sizeof(vorocells_ptr) * (*num_vcells));
    assert(*vorocells);
    (*vorocells)[nth_ncell] = (vorocells_ptr) malloc(sizeof(vorocells_t));
    assert((*vorocells)[nth_ncell]);

    // New edge(second-half, anti-clockwise) created and numbered in DCEL
    (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
    assert((*dcel)[*num_dcel]);
    tmp_C2C1_edge = (*dcel)[*num_dcel]; // Saving the pointer of this edge for future update
    (*vorocells)[nth_ncell]->idx_tower = nth_ncell;
    (*vorocells)[nth_ncell]->fst_edge = (*dcel)[*num_dcel];

    (*dcel)[*num_dcel]->idx_strt = idx_end_p; 
    (*dcel)[*num_dcel]->idx_end = idx_strt_p;
    (*dcel)[*num_dcel]->otr_edge = (*dcel)[*num_dcel-TWIN_EDGE];
    (*dcel)[*num_dcel]->corr_face = nth_ncell; // New face
    (*dcel)[*num_dcel]->idx_edge = new_edge_ptr->idx_edge; // anticlockwise half edge
    
    (*dcel)[*num_dcel - 1]->otr_edge = (*dcel)[*num_dcel]; // Assigning clockwise new hedge->ort_edge

    (*num_dcel)++; // Number of dcel update
    /**************************************************************************/
    // C1A2: New edge with strt_midpt to endpt of start_edge 
    
    if (strt_dup == FALSE) { // if the DCE is not exited
        *dcel = (edge_ptr*) realloc(*dcel, sizeof(edge_ptr) * 
                                    (*num_dcel + NEW_HEDGE_1));
        assert(*dcel);
        (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
        assert((*dcel)[*num_dcel]);

        
        (*dcel)[*num_dcel]->idx_strt = idx_strt_p; // idex_startpt
        (*dcel)[*num_dcel]->idx_end = (*dcel)[idx_strt_dcel]->idx_end; // idex_endpt
        (*dcel)[*num_dcel]->prev_edge = (*dcel)[*num_dcel-1]; // Prev_edge
        (*dcel)[*num_dcel]->next_edge = (*dcel)[idx_strt_dcel]->next_edge; // Next_edge
        if ((*dcel)[idx_strt_dcel]->otr_edge == NULL) { // Means no twins for strt_pt
            (*dcel)[*num_dcel]->otr_edge = NULL;
            strt_edge_twin = FALSE;
        }
        else { // C1A2 has twin edge == strt_edge has twin
            strt_edge_twin = TRUE;
        }
        (*dcel)[*num_dcel]->idx_edge = nth_edge; //
        (*dcel)[*num_dcel]->corr_face = nth_ncell; // New face
        
        tmp_C2C1_edge->next_edge = (*dcel)[*num_dcel]; // Upadge the next edge for previous edge
        tmp_C1A2_edge = (*dcel)[*num_dcel]; // Saving the pointer of this edge for future update

        (*num_dcel)++; // Number of dcel update
        nth_edge++;
    }
    else { // if the DCE is already exited

        nth_dcel = dcel_find(idx_strt_p, (*dcel)[idx_strt_dcel]->next_edge->idx_end, 
                                *num_dcel, dcel);

        tmp_C2C1_edge->next_edge = (*dcel)[nth_dcel]; // Upadge the 'next edge' for previous edge
        
        tmp_C1A2_edge = (*dcel)[nth_dcel]; // Saving the pointer of this edge for future update
        tmp_C1A2_edge->corr_face = nth_ncell;
        tmp_C1A2_edge->prev_edge = new_edge_ptr->otr_edge;
    }
    
  
    // B1C2: New edge from endpt of A2B1 to end_mitpt
     
    if (end_dup == FALSE) { // If the edge where the end point locates is not exited
        *dcel = (edge_ptr*) realloc(*dcel, sizeof(edge_ptr) * 
                (*num_dcel + NEW_HEDGE_1));
        assert(*dcel);
        (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
        assert((*dcel)[*num_dcel]);
        tmp_B1C2_edge = (*dcel)[*num_dcel];

        tmp_edge = tmp_C1A2_edge; // tmp = prev_edge(C1A2)
        while (tmp_edge->next_edge != (*dcel)[idx_end_dcel]) {
            tmp_edge->next_edge->prev_edge = tmp_edge;
            tmp_edge = tmp_edge->next_edge;
            tmp_edge->corr_face = nth_ncell; // Update new index of faces
        }
        tmp_edge->next_edge = tmp_B1C2_edge; // Update the next_edge for the prev_edge of ended_edge
        
        tmp_B1C2_edge->idx_strt = tmp_edge->idx_end;
        tmp_B1C2_edge->idx_end = idx_end_p;
        tmp_B1C2_edge->prev_edge = tmp_edge;
        tmp_B1C2_edge->next_edge = tmp_C2C1_edge;

        if ((*dcel)[idx_end_dcel]->otr_edge == NULL) {
            tmp_B1C2_edge->otr_edge = NULL;
            end_edge_twin = FALSE;
        }
        else { // B1C2 has twin edge == end_strt has twin
            end_edge_twin = TRUE;
        }

        tmp_B1C2_edge->idx_edge = nth_edge;
        tmp_B1C2_edge->corr_face = nth_ncell;
        tmp_C2C1_edge->prev_edge = tmp_B1C2_edge; // Update twin hedge -> prev_edge
        (*num_dcel)++;
        nth_edge++;
    }
    else {
    
        tmp_edge = tmp_C1A2_edge; 
        while (tmp_edge->next_edge != (*dcel)[idx_end_dcel]) {
            tmp_edge->next_edge->prev_edge = tmp_edge;
            tmp_edge = tmp_edge->next_edge;
            tmp_edge->corr_face = nth_ncell; // Update new index of faces
        }
        tmp_B1C2_edge = tmp_edge->next_edge;
        tmp_B1C2_edge->corr_face = nth_ncell;
        tmp_B1C2_edge->next_edge = new_edge_ptr->otr_edge;
        
    }
    
    
    // Processing the strated edge

    // The start edge cannot be C1A2, this situation occurs when there a point
    // already existed
    if ((*dcel)[idx_strt_dcel] != tmp_C1A2_edge) {
        (*dcel)[idx_strt_dcel]->idx_end = tmp_C1A2_edge->idx_strt; // Change the idx of end_point of strt_edge
        (*dcel)[idx_strt_dcel]->next_edge = new_edge_ptr; 
    }
    

    if (strt_edge_twin == TRUE) { // Means twins edge exited for strt_pt
            /* changing data for twin edge, maybe into a function*/
        
        edge_ptr strt_otr_edge = (*dcel)[idx_strt_dcel]->otr_edge;
        
        // First half idx_strt (starting point changed & prev_edge)
        strt_otr_edge->idx_strt = new_edge_ptr->idx_strt;

        // Second half (ending point changed)
        *dcel = (edge_ptr*) realloc(*dcel, sizeof(edge_ptr) 
                    * (*num_dcel + NEW_HEDGE_1));
        assert(*dcel);
        (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
        assert((*dcel)[*num_dcel]);
        edge_ptr sndhlf_strt_edge = (*dcel)[*num_dcel];
        
        sndhlf_strt_edge->idx_strt = tmp_C1A2_edge->idx_end;
        sndhlf_strt_edge->idx_end = tmp_C1A2_edge->idx_strt;
        sndhlf_strt_edge->next_edge = strt_otr_edge;
        sndhlf_strt_edge->prev_edge = strt_otr_edge->prev_edge;
        strt_otr_edge->prev_edge->next_edge = sndhlf_strt_edge;
        strt_otr_edge->prev_edge = sndhlf_strt_edge; // Update first half->prev_edge
        sndhlf_strt_edge->otr_edge = tmp_C1A2_edge;
        tmp_C1A2_edge->otr_edge = sndhlf_strt_edge;
        sndhlf_strt_edge->idx_edge = tmp_C1A2_edge->idx_edge;
        sndhlf_strt_edge->corr_face = strt_otr_edge->corr_face;

        (*num_dcel)++;

    }

    // Processing the ended edge
    
    if ((*dcel)[idx_end_dcel] != tmp_B1C2_edge) {
        (*dcel)[idx_end_dcel]->idx_strt = tmp_B1C2_edge->idx_end; // Change the idx of start_point of end_edge
        (*dcel)[idx_end_dcel]->prev_edge = new_edge_ptr; 
    }
    
    if (end_edge_twin == TRUE) { // Means twins edge exited for end_pt
        /* changing data for twin edge*/
        edge_ptr end_otr_edge = (*dcel)[idx_end_dcel]->otr_edge;

        // First half idx_end ending point changed & prev_edge)
        end_otr_edge->idx_end = new_edge_ptr->idx_end;

        // Second half (ending point changed)
        *dcel = (edge_ptr*) realloc(*dcel, sizeof(edge_ptr) 
                    * (*num_dcel + NEW_HEDGE_1));
        assert(*dcel);
        (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
        assert((*dcel)[*num_dcel]);
        edge_ptr sndhlf_end_edge = (*dcel)[*num_dcel];

        sndhlf_end_edge->idx_strt = tmp_B1C2_edge->idx_end;
        sndhlf_end_edge->idx_end = tmp_B1C2_edge->idx_strt;
        sndhlf_end_edge->prev_edge = end_otr_edge;
        sndhlf_end_edge->next_edge = end_otr_edge->next_edge;
        end_otr_edge->next_edge->prev_edge = sndhlf_end_edge;
        end_otr_edge->next_edge = sndhlf_end_edge; // Update first half->next_edge
        sndhlf_end_edge->otr_edge = tmp_B1C2_edge;
        tmp_B1C2_edge->otr_edge = sndhlf_end_edge;
        sndhlf_end_edge->idx_edge = tmp_B1C2_edge->idx_edge;
        sndhlf_end_edge->corr_face = end_otr_edge->corr_face;

        (*num_dcel)++;
    }
    
    // Reassign the vorocells list
    for (int f = 0; f < *num_vcells; f++) {
        for (int j = 0; j < *num_dcel; j++) {
            if ((*dcel)[j]->corr_face == f) {
                (*vorocells)[f]->fst_edge = (*dcel)[j];
                break;
            }
        }
    }
    
    vorocells_update(   *num_vcells, vorocells, 
                        *num_dcel, dcel, 
                        *num_tower, towers, 
                        vertics);

    printf("In split_polygon: \n");
    for (int i = 0; i < *num_dcel; i ++) {
        printf("%dth address = %p, idx_strt = %d, idx_end = %d, prev_edge = %p, "
                "next_edge = %p, otr_edge = %p, corr_face = %d, idx_edge = %d\n", 
                i, (*dcel)[i], (*dcel)[i]->idx_strt, (*dcel)[i]->idx_end, 
                (*dcel)[i]->prev_edge, (*dcel)[i]->next_edge, 
                (*dcel)[i]->otr_edge, (*dcel)[i]->corr_face, 
                (*dcel)[i]->idx_edge);
    }
    // Vertics print
    for (int i = 0; i < *num_vertics; i++) {
        printf("Vertice %d: (%f, %f)\n", i, (*vertics)[i]->x,(*vertics)[i]->y);
    }

    printf("Split_polyton done!\n");

}



/******************************************************************************/

void 
strt_n_end_edge(int* idx_strt_dcel, int* idx_end_dcel,
                 int* num_dcel, edge_ptr **dcel, itct_p_t** itct_ps) {
    int i, j;
    int strt_edge, end_edge;

    strt_edge = (*itct_ps)[0].idx_edge;
    end_edge = (*itct_ps)[1].idx_edge;
    printf("strt edge = %d and end edge = %d\n", strt_edge, end_edge);
    

    for (i = 0; i < *num_dcel; i++) {
        if ( (*dcel)[i]->idx_edge == strt_edge ) {

            for (j = 0; j < *num_dcel; j++) {
                if ( (*dcel)[j]->idx_edge == end_edge ) {

                    if ((*dcel)[i]->corr_face == (*dcel)[j]->corr_face) {
                        *idx_strt_dcel = i;
                        *idx_end_dcel = j;
                        return;
                    }
                }
            }
        }
    }

    printf("Splliting line is crossing some exciting line, Error!\n");
    exit(EXIT_FAILURE);
}

/******************************************************************************/
/*
    strt_end_edge_chk() function:
    Description:
        This function is used to check if the strt intersection point is at the  
        end of starting edge, end intersectino point is at the start of ending 
        edge.
    Inputs:
        
    Outputs:
        Adjust the pointers of index of starting edge and ending edge if needed
*/

void 
strt_end_edge_chk(int* idx_dcel, int codn, int num_dcel, edge_ptr **dcel, int nth) {
    edge_ptr crnt_edge;
    crnt_edge = (*dcel)[*idx_dcel];

    if (codn) { // Adjusting Starting edge
        while(crnt_edge->idx_end != nth) {
            crnt_edge = crnt_edge->next_edge;
        }
        
    }
    else { // Adjusting Ending edge
        while(crnt_edge->idx_strt != nth) {
            crnt_edge = crnt_edge->next_edge;
        }
    }
    
    *idx_dcel = dcel_find(crnt_edge->idx_strt, crnt_edge->idx_end, num_dcel, dcel);
}

/******************************************************************************/