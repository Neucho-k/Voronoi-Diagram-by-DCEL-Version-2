#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"topo_strt.h"

void 
vorocells_create(int num_towers, vorocells_ptr **vorocells){
    int i;

    *vorocells = (vorocells_ptr*) realloc(*vorocells, sizeof(vorocells_ptr) * (num_towers));
    assert(*vorocells);

    for (i = 0; i < num_towers; i++) {
        (*vorocells)[i] = (vorocells_ptr) malloc(sizeof(vorocells_t));
        assert((*vorocells)[i]);

        (*vorocells)[i]->idx_tower = i;
        (*vorocells)[i]->fst_edge = NULL;
    }
    
}