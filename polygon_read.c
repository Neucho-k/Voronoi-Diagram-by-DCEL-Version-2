#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"topo_strt.h"

/*
    polygon_read() function:
    Inputs: 
        pointer to the filename of 'polygon_xxx.txt'
        pointer to integer of number of vertics
        pointer to structure of vertics
    Outputs:
        Saving vertics into a list of pointer to point_t
*/
void 
polygon_read(char* filename, int*num, point_ptr **location){
    double x, y;
    FILE *file = fopen(filename, "r");

    while(fscanf(file, "%lf %lf", &x, &y) != EOF) {
        
        // Allocation memory
        *location = (point_ptr*) realloc(*location,sizeof(point_ptr)*(*num+1));
        assert(*location);
        (*location)[*num] = (point_ptr) malloc(sizeof(point_t));
        assert((*location)[*num]);

        // Assinging x & y values for each vertex of polygon
        (*location)[*num]->x = x;
        (*location)[*num]->y = y;

        // Updating numbeb of points
        (*num)++; 
    }

    fclose(file);   
    printf("Polygen Read Completed!\n");
}