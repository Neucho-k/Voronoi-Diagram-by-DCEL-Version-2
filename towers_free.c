#include<stdio.h>
#include<stdlib.h>
#include"watchtowers.h"

void 
towers_free(int* num, tower_ptr **towers) {
    int i;

    for (i = 0; i < *num; i++) {
        free((*towers)[i]->ID);
        free((*towers)[i]->postcode);
        free((*towers)[i]->mang);
        free((*towers)[i]);
    }

    free(*towers);
    printf("Free towers done!\n"); 
}