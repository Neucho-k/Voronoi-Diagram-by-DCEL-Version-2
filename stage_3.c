#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<math.h>
#include"watchtowers.h"
#include"topo_strt.h"
#include"stages.h"
#include"calc.h"

#define NUM_PT 2
#define TRUE 1
#define FALSE 0
#define NEW_CELL 1

void 
stage_3(char* output_filename, int* num_vertcs, point_ptr **vertics, 
            int* num_dcel, edge_ptr **dcel, int* num_towers, tower_ptr **towers,
            vorocells_ptr **vorocells, info_ptr **info_array){

    int i, t, p, bd, nebr_wt, num_twedge, nth_vertex, nth_dcel;
    int num_vcells = 1;
    int twin_exist = FALSE;
    FILE *output_file = fopen(output_filename, "w");

    point_t p1, p2;
    edge_ptr tmp_edge, crnt_edge, nwform_edge;

    // Initialization of vorocells list
    (*vorocells)[0]->idx_tower = 0;
    (*vorocells)[0]->fst_edge= (*dcel)[0];

    // Initialization of info_array
    (*info_array) = (info_ptr*) realloc(*info_array, sizeof(info_ptr) * (*num_towers));
    assert(*info_array);
    for (int q = 0; q < *num_towers; q++) {
        (*info_array)[q] = (info_ptr) malloc(sizeof(info_t));
        assert((*info_array)[q]);
    }

    // Allocate memory for intersection points list
    itct_p_t *itct_ps;
    itct_ps = (itct_p_t*) malloc(sizeof(itct_p_t) * NUM_PT);
    assert(itct_ps);

    // Each watchtower
    for (t = 1; t < *num_towers; t++){
        printf("\n/*******Tower %d: **************/\n", t);

        /*************************FIRST STEP***********************************/
        // Searching corresponding voronic cell for each tower
        for (i = 0; i < num_vcells; i++){
            if (tower_alloc_chk(&((*vorocells)[i]->fst_edge), &(*towers)[t], vertics) == TRUE) {
                break;
            }
        }

        // Finding the intersection points
        p1.x = (*towers)[(*vorocells)[i]->idx_tower]->x;
        p1.y = (*towers)[(*vorocells)[i]->idx_tower]->y;
        p2.x = (*towers)[t]->x;
        p2.y = (*towers)[t]->y;
        printf("\n");
        printf("p1: (%f, %f)\n", p1.x, p1.y);
        printf("p2: (%f, %f)\n", p2.x, p2.y);

        bisector(&p1, &p2, &itct_ps, vertics, (*vorocells)[i]->fst_edge);

        // Vertics print
        printf("Under bisector 1: \n");
        for (i = 0; i < *num_vertcs; i++) {
            printf("Vertice %d: (%f, %f)\n", i, (*vertics)[i]->x,(*vertics)[i]->y);
        }

        printf("Each tower: \n");
        printf("itct_ps[0].idx_edge = %d\n", itct_ps[0].idx_edge);
        printf("itct_ps[1].idx_edge = %d\n", itct_ps[1].idx_edge);
        printf("itct_ps[0]: (%f, %f)\n", itct_ps[0].x, itct_ps[0].y);
        printf("itct_ps[1]: (%f, %f)\n", itct_ps[1].x, itct_ps[1].y);

        // Split the polygen
        split_polygon(&num_vcells, vorocells, num_vertcs, vertics, num_dcel, 
                        dcel, &t, towers, &itct_ps);

        /*************************SECOND STEP**********************************/
        // If the intersction points located on DCEs
        printf("\nSecond Step: \n");
        num_twedge = 0;
        for (p = 0; p < NUM_PT; p++) {
            if ( (*dcel)[itct_ps[p].idx_edge]->otr_edge == NULL ) {
                printf("p = %d, No twin edge.\n", p);
            }
            else {
                printf("p = %d, Has twin edge.\n", p);
                twin_exist = TRUE;
                num_twedge++;
                
                // Find the neighbour watchtower as nebr_wt
                for (i = 0; i < *num_dcel; i++) {
                    if ((*dcel)[i]->idx_edge == itct_ps[p].idx_edge) {
                        tmp_edge = (*dcel)[i];
                    }
                }

                if (p == 0){ // First intersection point
                    strt_edge_find(&itct_ps[p], &tmp_edge, vertics);      
                }    
                else {
                    end_edge_find(&itct_ps[p], &tmp_edge, vertics);
                }
                printf("tmp_edge = %p\n", tmp_edge);
                nebr_wt = (*vorocells)[tmp_edge->otr_edge->corr_face]->idx_tower;
                

                // Finding site bisector 
                p1.x = (*towers)[nebr_wt]->x;
                p1.y = (*towers)[nebr_wt]->y;

                itct_p_t *itct_ps2;
                itct_ps2 = (itct_p_t*) malloc(sizeof(itct_p_t) * NUM_PT);
                assert(itct_ps2);

                printf("p1: (%f, %f)\n", p1.x, p1.y);
                printf("p2: (%f, %f)\n", p2.x, p2.y);

                bisector(&p1, &p2, &itct_ps2, vertics, tmp_edge->otr_edge);
                printf("itct_ps2[0].x = %f, y = %f\n", itct_ps2[0].x, itct_ps2[0].y);
                printf("itct_ps2[1].x = %f, y = %f\n", itct_ps2[1].x, itct_ps2[1].y);
            
                split_polygon(&num_vcells, vorocells, num_vertcs, vertics, num_dcel, 
                    dcel, &t, towers, &itct_ps2);
                
                free(itct_ps2);
                //break;

            }
        }


        /*******************THIRD STEP: ELIMINATION***************************/
        int *delete_face = NULL, num_delete = 0;

        // Intersection points on DCE (at least one DCE)
        if (twin_exist == TRUE) {
            //num_twedge

            // Locating to the new form edge of current voronoi cell
            // Error here
            nth_vertex = vertex_find(*num_vertcs, vertics, itct_ps[0]);
            int idx_strt_p = nth_vertex;
            nth_vertex = vertex_find(*num_vertcs, vertics, itct_ps[1]);
            int idx_end_p = nth_vertex;
            printf("idx_strt_p = %d, idx_end_p = %d\n", idx_strt_p, idx_end_p);

            nth_dcel = dcel_find(idx_strt_p, idx_end_p, *num_dcel, dcel);

            crnt_edge = (*dcel)[nth_dcel];

            if (crnt_edge->corr_face != t) {
                crnt_edge = crnt_edge->otr_edge;
            }

            printf("crnt_edge = %p\n", crnt_edge);

            edge_ptr itct_edge = crnt_edge;
            edge_ptr fst_edge = crnt_edge, next_edge;

            while (crnt_edge->next_edge != fst_edge) {
                next_edge = crnt_edge->next_edge;
                
                if (next_edge->otr_edge != NULL) { // Meet DCE
                    nwform_edge = next_edge->otr_edge->next_edge;

                    bd = FALSE;
                    bounded_test(&nwform_edge, &bd); // Bounded edge test

                    if (bd) {
                        printf("bounded!\n");
                    }
                    else {
                        printf("unbounded!\n");

                        // Record the num of face that should be deleted later
                        delete_face = (int*) realloc(delete_face, 
                                                sizeof(int) * (num_delete + 1));
                        assert(delete_face);
                        delete_face[num_delete] = nwform_edge->corr_face;
                        printf("next-edge = %p\n", next_edge);
                        printf("nwform_edge = %p has face %d\n", nwform_edge,nwform_edge->corr_face);
                        num_delete++;

                        ubd_edge_update(&nwform_edge, &crnt_edge, &itct_edge);

                        // Hit the origin
                        if (nwform_edge->idx_end == itct_edge->idx_strt) { 
                            break;
                        }
                    }
 
                }

                crnt_edge = crnt_edge->next_edge;
                printf("End of while loop: crnt_edge = %p\n", crnt_edge);

            }
            //Printing DCEL
            printf("Before elimating: \n");
            for (int i = 0; i < *num_dcel; i ++) {
                printf("%dth address = %p, idx_strt = %d, idx_end = %d, prev_edge = %p, "
                        "next_edge = %p, otr_edge = %p, corr_face = %d, idx_edge = %d\n", 
                        i, (*dcel)[i], (*dcel)[i]->idx_strt, (*dcel)[i]->idx_end, 
                        (*dcel)[i]->prev_edge, (*dcel)[i]->next_edge, 
                        (*dcel)[i]->otr_edge, (*dcel)[i]->corr_face, 
                        (*dcel)[i]->idx_edge);
                    
            }

            // Elimating edges
            for (int i = 0; i < num_delete; i++) {
                printf("Delete_face[%d] = %d\n", i, delete_face[i]);
                edge_elimination(delete_face[i], num_dcel, dcel);
            }

            //Printing DCEL
            printf("After elimating: \n");
            for (int i = 0; i < *num_dcel; i ++) {
                printf("%dth address = %p, idx_strt = %d, idx_end = %d, prev_edge = %p, "
                        "next_edge = %p, otr_edge = %p, corr_face = %d, idx_edge = %d\n", 
                        i, (*dcel)[i], (*dcel)[i]->idx_strt, (*dcel)[i]->idx_end, 
                        (*dcel)[i]->prev_edge, (*dcel)[i]->next_edge, 
                        (*dcel)[i]->otr_edge, (*dcel)[i]->corr_face, 
                        (*dcel)[i]->idx_edge);
                    
            }

        }

        free(delete_face);

        for (p = 0; p <num_twedge; p++) {
            num_vcells--;
            free((*vorocells)[num_vcells]);
        }
        
        *vorocells = (vorocells_ptr*) realloc (*vorocells, sizeof(vorocells_ptr) * num_vcells);
        assert(*vorocells);
    
    }

    // Reassign the vorocells list
    for (int f = 0; f < num_vcells; f++) {
        for (int j = 0; j < *num_dcel; j++) {

            if ((*dcel)[j]->corr_face == f) { 

                (*vorocells)[f]->fst_edge = (*dcel)[j];
                printf("(*vorocells)[%d]->fst_edge = %p\n", f, (*vorocells)[f]->fst_edge);
                break;
            }
        }
    }

    // Cycle check
    for (int i = 0; i < num_vcells; i++) {
        printf("At %dth cells: ", i);
        cycle_chk(&(*vorocells)[i]->fst_edge);
    }

    // Calculating the diameter of each cell
    diameter_calc(&output_file, num_vcells, vorocells, towers, vertics, info_array);



    /******************************DISPLAY INFO********************************/
    //Printing DCEL
    for (int i = 0; i < *num_dcel; i ++) {
        printf("%dth address = %p, idx_strt = %d, idx_end = %d, prev_edge = %p, "
                "next_edge = %p, otr_edge = %p, corr_face = %d, idx_edge = %d\n", 
                i, (*dcel)[i], (*dcel)[i]->idx_strt, (*dcel)[i]->idx_end, 
                (*dcel)[i]->prev_edge, (*dcel)[i]->next_edge, 
                (*dcel)[i]->otr_edge, (*dcel)[i]->corr_face, 
                (*dcel)[i]->idx_edge);
    }

    // Printing vertics
    for (i = 0; i < *num_vertcs; i++) {
        printf("Vertice %d: (%f, %f)\n", i, (*vertics)[i]->x,(*vertics)[i]->y);
    }
    
    free(itct_ps);
    fclose(output_file);

    printf("Stage_3 is done!\n");
}
/******************************************************************************/