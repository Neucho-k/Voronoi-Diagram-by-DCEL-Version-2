#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"watchtowers.h"

void 
towers_read(char* filename, int* num, tower_ptr **towers) {
    char *line = NULL, *token = NULL;
    size_t lineBufferLength = 0;
    const char s[2] = ",";
    FILE *file = fopen(filename, "r");

    // Removing the heading
    getline(&line, &lineBufferLength, file); 

    while(getline(&line, &lineBufferLength, file) > 0){
        
        // Reading lines of data from dataset.csv
        *towers = (tower_ptr*) realloc(*towers, sizeof(tower_ptr) * (*num + 1));
        assert(*towers);
        (*towers)[*num] = (tower_ptr) malloc(sizeof(tower_t));
        assert((*towers)[*num]);

        // ID
        token = strtok(line, s);
        (*towers)[*num]->ID = (char*) malloc( sizeof(char) * (strlen(token) + 1));
        assert((*towers)[*num]->ID);
        strcpy((*towers)[*num]->ID, token);

        // Postcode
        token = strtok(NULL, s);
        (*towers)[*num]->postcode = (char*) malloc( sizeof(char) * (strlen(token) + 1));
        assert((*towers)[*num]->postcode);
        strcpy((*towers)[*num]->postcode, token);

        // Population
        token = strtok(NULL, s);
        (*towers)[*num]->pop = atoi(token);

        // Manager
        token =strtok(NULL, s);
        (*towers)[*num]->mang = (char*) malloc( sizeof(char) * (strlen(token) + 1));
        assert((*towers)[*num]->mang);
        strcpy((*towers)[*num]->mang, token);

        // x
        token = strtok(NULL, s);
        (*towers)[*num]->x = atof(token);

        // y
        token = strtok(NULL, s);
        (*towers)[*num]->y = atof(token);

        // Free stout and update the number of watchtowers
        fflush(stdout);
        (*num)++;
    }

    // Close the file
    fflush(stdout);
    fclose(file);
    free(line);

    printf("Towers_read complete!\n");
}