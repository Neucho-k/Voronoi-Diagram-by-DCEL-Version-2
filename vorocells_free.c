#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"topo_strt.h"

void 
vorocells_free(int num_vcells, vorocells_ptr **vorocells){
    int i;

    for (i = 0; i < num_vcells; i++) {
        free((*vorocells)[i]);
    }

    free(*vorocells);
}