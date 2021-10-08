/* COMP20003 Algorithm and Data Structure - Assignment_2 */
/* Lichuan Li, 1074497 */
/* Started on 05/09/2021 */

/* Programming is fun! Always:) */

#include<stdio.h>
#include<stdlib.h>
#include"watchtowers.h"
#include"topo_strt.h"
#include"stages.h"
#include"calc.h"

#define STAGE 1
#define STAGE_1 1
#define STAGE_2 2
#define STAGE_3 3
#define STAGE_4 4
#define NUM_ARGT_STAGE_1 3
#define NUM_ARGT_STAGE_2 4
#define NUM_ARGT_STAGE_3 4
#define PP_INPUT 2
#define STG1_OUTPUT 3
#define STG2_OUTPUT 4
#define STG3_OUTPUT 4
#define STG4_OUTPUT 4
#define POLYGON_READ 3
#define TOWER_READ 2

int 
main(int argc, char** argv) {

    // Initialization
    int num_vertices = 0, num_dcel = 0, num_towers = 0;

    point_ptr* vertics = NULL;
    edge_ptr* dcel = NULL;
    tower_ptr* towers = NULL;
    vorocells_ptr* vorocells = NULL;
    info_ptr* info_array = NULL;


    // Processing Stage 1
    if (atoi(argv[STAGE]) == STAGE_1) {

        if (argc - 1 < NUM_ARGT_STAGE_1){
            printf("Less than three arguments are detected for Stage 1!\n");
            exit(EXIT_FAILURE);
        }
        else {
            printf("Stage 1 Processing...\n");
            stage_1(argv[PP_INPUT], argv[STG1_OUTPUT]);
        }
    }


    // Processing Stage 2
    else if (atoi(argv[STAGE]) == STAGE_2) {

        if (argc - 1 < NUM_ARGT_STAGE_2){
            printf("Less than four arguments are detected for Stage 2!\n");
            exit(EXIT_FAILURE);
        }
        else {
            printf("Stage 2 Processing...\n");
            polygon_read(argv[POLYGON_READ], &num_vertices, &vertics);
            dcel_create(&num_vertices, &vertics, &num_dcel, &dcel);
            
            stage_2(argv[PP_INPUT], argv[STG2_OUTPUT], &vertics, &dcel);
        }
    }

    // Processing Stage 3
    else if (atoi(argv[STAGE]) == STAGE_3) {
        if (argc - 1 < NUM_ARGT_STAGE_3){
            printf("Less than four arguments are detected for Stage 3!\n");
            exit(EXIT_FAILURE);
        }
        else {
            printf("Stage 3 Processing...\n");
            towers_read(argv[TOWER_READ], &num_towers, &towers);
            polygon_read(argv[POLYGON_READ], &num_vertices, &vertics);
            dcel_create(&num_vertices, &vertics, &num_dcel, &dcel);
            vorocells_create(num_towers, &vorocells);

            stage_3(argv[STG3_OUTPUT], &num_vertices, &vertics, &num_dcel, &dcel, 
                        &num_towers, &towers, &vorocells, &info_array);
            
        }

    }

    // Processing Stage 4
    else if (atoi(argv[STAGE]) == STAGE_4) {
        if (argc - 1 < NUM_ARGT_STAGE_3){
            printf("Less than four arguments are detected for Stage 3!\n");
            exit(EXIT_FAILURE);
        }
        else {
            printf("Stage 4 Processing...\n");
            towers_read(argv[TOWER_READ], &num_towers, &towers);
            polygon_read(argv[POLYGON_READ], &num_vertices, &vertics);
            dcel_create(&num_vertices, &vertics, &num_dcel, &dcel);
            vorocells_create(num_towers, &vorocells);

            stage_3(argv[STG4_OUTPUT], &num_vertices, &vertics, &num_dcel, &dcel, 
                        &num_towers, &towers, &vorocells, &info_array);
            
            stage_4(argv[STG4_OUTPUT], &info_array, num_towers);

        }
    }

    // Freeing memory
    info_free(num_towers, &info_array);
    vorocells_free(num_towers, &vorocells);
    polygon_free(&num_vertices, &vertics);
    dcel_free(&num_dcel, &dcel);
    towers_free(&num_towers, &towers);

    return 0;
}
