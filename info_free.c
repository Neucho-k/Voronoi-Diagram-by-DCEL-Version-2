#include<stdio.h>
#include<stdlib.h>
#include"topo_strt.h"

void 
info_free(int num, info_ptr** info_array) {
    int i;

    for (i = 0; i < num; i++) {
        free((*info_array)[i]);
    }

    free(*info_array);
}