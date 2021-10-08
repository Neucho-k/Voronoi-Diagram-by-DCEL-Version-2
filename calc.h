void 
bisector_stg1(point_t *p1, point_t *p2, FILE **output_file);

void 
midpoint(point_t *mid_p, point_t *p1, point_t *p2);

double 
gradient_bisec(point_t *p1, point_t *p2);

void 
bisector(point_t *p1, point_t *p2, itct_p_t** itct_ps, 
                point_ptr **vertics, edge_ptr fst_edge);

double 
gradient(point_t *p1, point_t *p2);

void 
vrtln_check(point_ptr *tmp_strt, point_ptr *tmp_end, double itct_y, 
            itct_p_t** itct_ps, int *count, int idx_edge);

void 
hrzln_check(point_ptr *tmp_strt, point_ptr *tmp_end, double itct_x, 
            itct_p_t** itct_ps, int *count, int idx_edge);

int 
domain_check(point_ptr *tmp_strt, point_ptr *tmp_end, double *itct_x);

int 
range_check(point_ptr *tmp_strt, point_ptr *tmp_end, double *itct_y);

int 
tower_alloc_chk(edge_ptr* face, tower_ptr* tower, point_ptr** vertcs);

int 
yr_test_above(double x, double y, double m, double c);

int 
yr_test_below(double x, double y, double m, double c);

void 
vorocells_update(int num_vcells, vorocells_ptr** vorocells, 
                    int num_dcel, edge_ptr **dcel, 
                    int num_tower, tower_ptr** towers,
                    point_ptr** vertics);

int 
tf_find(vorocells_ptr** vorocells, int num_vcells, int idx_tower);

void 
strt_edge_find(itct_p_t *itct_ps, edge_ptr *tmp_edge, point_ptr **vertics);

void 
end_edge_find(itct_p_t *itct_ps, edge_ptr *tmp_edge, point_ptr **vertics);

int
vertics_check(itct_p_t itct_p, int num_vertics, point_ptr **vertics, int *nth);

void 
itct_pivrt(itct_p_t **itct_ps);

int
dcel_find(int idx_strt_p, int idx_end_p, int num_dcel, edge_ptr **dcel);

int 
vertex_find(int num_vertics, point_ptr** vertics, itct_p_t itct_p);

void 
corrf_update(edge_ptr* fst_edge, int nth);

void 
split_polygon(  int* num_vcells, vorocells_ptr** vorocells, 
                int* num_vertics, point_ptr **vertics, 
                int* num_dcel, edge_ptr **dcel, 
                int* num_tower, tower_ptr** towers,
                itct_p_t **itct_ps);

void 
bounded_test(edge_ptr *edge, int * bd);

void 
cycle_chk(edge_ptr *fst_edge);

void 
edge_elimination(int corr_face, int *num_dcel, edge_ptr **dcel);

void
ubd_edge_update(edge_ptr *nwform_edge, edge_ptr *crnt_edge, edge_ptr *itct_edge);

void 
diameter_calc(FILE **output_file, int num_vcells, vorocells_ptr **vorocells, 
                tower_ptr **towers, point_ptr **vertics, info_ptr **info_array);

double
distance_calc(point_ptr tower_p, point_ptr vertex_p);