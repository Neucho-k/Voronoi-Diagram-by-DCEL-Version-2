#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"topo_strt.h"
#include"watchtowers.h"
#include"stages.h"
#include"calc.h"

#define NUM_PT 2

void 
stage_2(char* pp_filename, char* output_filename, 
            point_ptr **vertics, edge_ptr **dcel){

    point_t p1, p2;
    edge_ptr fst_edge = **dcel;

    FILE *pp_file = fopen(pp_filename, "r");
    FILE *output_file = fopen(output_filename, "w");

    // Allocate memory for intersection points list
    itct_p_t *itct_ps;
    itct_ps = (itct_p_t*) malloc(sizeof(itct_p_t) * NUM_PT);
    assert(itct_ps);

    // Reading point pairs data
    while(fscanf(pp_file, "%lf %lf %lf %lf", 
            &(p1.x), &(p1.y), &(p2.x), &(p2.y)) != EOF) {
        
        bisector(&p1, &p2, &itct_ps, vertics, fst_edge);
        stg2_write(&output_file, &itct_ps);
    }

    free(itct_ps);
    fclose(pp_file); 
    fclose(output_file); 

    printf("Stage_2 is done!\n");
}

/******************************************************************************/
/*
    stg2_write() function:
    Description:
        Writing the intersection point of bisector and polygons into file
    Inputs: 
        pointer to output file
        pointer to intersection points
    Outputs:
        From Edge 0 (0.0 ,0.0) to Edge 0 (0.0, 0.0)
*/

void 
stg2_write(FILE **output_file, itct_p_t** itct_ps){

    fprintf(*output_file, "From Edge %d (%f, %f) to Edge %d (%f, %f)\n", 
           (*itct_ps)[0].idx_edge, (*itct_ps)[0].x, (*itct_ps)[0].y, 
           (*itct_ps)[1].idx_edge, (*itct_ps)[1].x, (*itct_ps)[1].y);
}

/***********************************END****************************************/