#include<stdio.h>
#include<stdlib.h>
#include"topo_strt.h"
#include"watchtowers.h"
#include"stages.h"
#include"calc.h"

/*  stag_1() function:
    Inputs: 
        pointer to the filename of 'pp_inside.txt'
        pointer to the filename of '1-outputfile.txt'
    Outputs: 
        Wrting biosector into file '1-outputfile.txt'
*/  

void 
stage_1(char* pp_filename, char* output_filename){
    point_t p1, p2;

    FILE *pp_file = fopen(pp_filename, "r");
    FILE *output_file = fopen(output_filename, "w");

    // Reading point pairs data
    while(fscanf(pp_file, "%lf %lf %lf %lf", 
            &(p1.x), &(p1.y), &(p2.x), &(p2.y)) != EOF) {

        bisector_stg1(&p1, &p2, &output_file);
    }

    fclose(pp_file); 
    fclose(output_file); 
    printf("Stage_1 is done!\n");
}

/***********************************END****************************************/