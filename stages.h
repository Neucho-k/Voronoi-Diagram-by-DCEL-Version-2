void 
stage_1(char* pp_filename, char* output_filename);

void 
stage_2(char* pp_filename, char* output_filename, 
            point_ptr **vertics, edge_ptr **dcel);

void 
stg2_write(FILE **output_file, itct_p_t** itct_ps);

void 
stage_3(char* output_filename, 
        int* num_vertcs, point_ptr **vertics, 
        int* num_dcel, edge_ptr **dcel, 
        int* num_towers, tower_ptr **towers,
        vorocells_ptr **vorocells, info_ptr **info_array);

void 
stage_4(char* output_filename, info_ptr** info_array, int num_tower);