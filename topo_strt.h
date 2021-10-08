
typedef struct {
    double x, y;
}point_t;

typedef struct edge{
    int idx_end, idx_strt, idx_edge;
    struct edge *next_edge, *prev_edge, *otr_edge;
    int corr_face;
}edge_t;

typedef struct {
    double x, y;
    int idx_edge;
}itct_p_t;

typedef struct {
    int idx_tower;
    edge_t* fst_edge;
}vorocells_t;

typedef struct {
    char *info;
    double distn;
}info_t;

typedef point_t* point_ptr;
typedef edge_t* edge_ptr;
typedef vorocells_t* vorocells_ptr;
typedef info_t* info_ptr;

void 
polygon_read(char* filename, int*num, point_ptr **location);

void 
polygon_free(int* num, point_ptr **location);

void 
dcel_create(int* num_vertcs, point_ptr **vertics, int* num_dcel, 
                edge_ptr **location);

void 
dcel_free(int* num, edge_ptr **dcel);

void 
faces_create(edge_ptr **dcel, int* num_faces, edge_ptr** faces);

void 
faces_free(int* num_faces, edge_ptr** faces);

void 
vorocells_create(int num_towers, vorocells_ptr **vorocells);

void 
vorocells_free(int num_towers, vorocells_ptr **vorocells);

void 
info_free(int num, info_ptr** info_array);
