#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"topo_strt.h"

#define NUM_PT 2
#define TRUE 1
#define FALSE 0
#define NEW_CELL 1

void 
stage_4(char* output_filename, info_ptr** info_array, int num_tower){ 

    int num_line = 0, len = 0;
    int i, j;
    double tmp_dstin;
    char* tmp_info;

    char *line = NULL;
    size_t lineBufferLength = 0;
    FILE *file = fopen(output_filename, "r");

    while(getline(&line, &lineBufferLength, file) > 0){
        len = strlen(line);
        
        (*info_array)[num_line]->info = (char*) malloc(sizeof(char) * len);
        assert((*info_array)[num_line]->info);

        strcpy((*info_array)[num_line]->info, line);
        printf("array_info[num_line]->info = %s", (*info_array)[num_line]->info);
        printf("array_info[num_line]->distn = %f\n", (*info_array)[num_line]->distn);

        // Free stout and update the number of watchtowers
        fflush(stdout);
        (num_line)++;

    }
    fclose(file);

    // Insertion sort
    for (i = 1; i < num_tower; i++) {
        tmp_info = (*info_array)[i]->info; 
        tmp_dstin = (*info_array)[i]->distn; 
        j = i - 1;

        while (j>= 0 && (*info_array)[j]->distn > tmp_dstin) {
            (*info_array)[j+1]->info = (*info_array)[j]->info;
            (*info_array)[j+1]->distn = (*info_array)[j]->distn;
            j = j - 1;
        }

        (*info_array)[j+1]->info = tmp_info;
        (*info_array)[j+1]->distn = tmp_dstin;
    }

    // Writing into outpuf file
    FILE *f_file = fopen(output_filename, "w");

    for (i = 0; i < num_tower; i++) {
        fprintf(f_file, (*info_array)[i]->info);
    }
    
    // Close the file
    fflush(stdout);
    fclose(file);
    free(line);

    printf("Stage4 complete!\n");
    
}