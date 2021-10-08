/* This file is to read the watch tower data */

typedef struct {
    char* ID;
    char* postcode;
    int pop;
    char* mang;
    double x;
    double y;
}tower_t;

typedef tower_t* tower_ptr;

void    
towers_read(char* filename, int* num, tower_ptr **towers);

void 
towers_free(int* num, tower_ptr **towers);