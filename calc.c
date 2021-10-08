#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<math.h>
#include"watchtowers.h"
#include"topo_strt.h"
#include"calc.h"

#define HALF 0.5
#define NUM_PT 2
#define TRUE 1
#define FALSE 0

/*
    bisector_stg1() function:
    Inputs: 
        pointer to point1, pointer to point2
    Outputs:
*/

void 
bisector_stg1(point_t *p1, point_t *p2, FILE **output_file){
    point_t mid_p;

    midpoint(&mid_p, p1, p2);

    // Condition 1: bisector is perpendicular to x-axis
    if (p1->y == p2->y) {
        fprintf(*output_file, "x = %f\n", mid_p.x);
    }

    // Condition 2: bisector is not perpendicular to x-axis
    else {
        fprintf(*output_file, "y = %f * (x - %f) + %f\n", 
                    gradient_bisec(p1, p2), mid_p.x, mid_p.y);
    }

}

/******************************************************************************/
/*
    midpoint() function:
    Inputs: 
        pointer to midpoint, pointer to point1, pointer to point2
    Outputs: 
        Assigning x and y values of midpoint
*/

void 
midpoint(point_t *mid_p, point_t *p1, point_t *p2){
    mid_p->x = (p1->x + p2->x) * HALF;
    mid_p->y = (p1->y + p2->y) * HALF;
}

/******************************************************************************/
/* 
    gradient() functions:
    Inputs: 
        pointer to point1, pointer to point2
    Outputs: 
        Returning the gradient of bisector
*/

double 
gradient_bisec(point_t *p1, point_t *p2) {
    if (p2->x == p1->x) {
        return (double) 0;
    }
    else {
        return -(p2->x - p1->x) / (p2->y - p1->y);
    } 
}

/******************************************************************************/
/*
    bisector() function:
    Description:
        Taking two points to find the bisector, and hence find the intersection
        points which intersect with edges/polygons
    Inputs: 
        pointer to point1 (location of watchtower 1)
        pointer to point2 (location of watchtower 2)
        pointer to intersection points
        pointer to vertics list
        pointer to first edge
    Outputs:
        Write the information of two intersection points into
        the input pointer 'pointer to intersection points'
*/

void 
bisector(point_t *p1, point_t *p2, itct_p_t** itct_ps, 
                point_ptr **vertics, edge_ptr fst_edge){

    int idx_edge, count = 0;
    double m_bisector, m_pgedge, c_bisector, c_pgedge, tmp_x, tmp_y;

    point_t mid_p;
    point_ptr tmp_strt, tmp_end;
    edge_ptr current_edge;

    midpoint(&mid_p, p1, p2);
    current_edge = fst_edge;

    // Condition 1, if bisector is perpendicular to x-axis
    if (p1->y == p2->y) {
        while (count != NUM_PT && (current_edge !=fst_edge || !count)) {
            tmp_strt = (*vertics)[current_edge->idx_strt];
            tmp_end = (*vertics)[current_edge->idx_end];
            idx_edge = current_edge->idx_edge;

            // Whether the x-intersection is in the domain
            hrzln_check(&tmp_strt, &tmp_end, mid_p.x, itct_ps, &count, idx_edge);

            current_edge = current_edge->next_edge;
        }
    }

    // Condition 2, if bisector is parallel to y-axis
    else if (p1->x == p2->x) {
        while (count != NUM_PT && (current_edge !=fst_edge || !count)) {
            tmp_strt = (*vertics)[current_edge->idx_strt];
            tmp_end = (*vertics)[current_edge->idx_end];
            idx_edge = current_edge->idx_edge;

            // Whether the y-intersection is in the range
            vrtln_check(&tmp_strt, &tmp_end, mid_p.y, itct_ps, &count, idx_edge);

            current_edge = current_edge->next_edge;
        }
    }

    // Condition 3, other than the gradient of bisector is zero or infinity
    else {
        m_bisector = gradient_bisec(p1, p2);
        c_bisector = m_bisector * (-mid_p.x) + mid_p.y;
    
        while (count != NUM_PT && (current_edge !=fst_edge || !count)) {
            tmp_strt = (*vertics)[current_edge->idx_strt];
            tmp_end = (*vertics)[current_edge->idx_end];

            // Subcondition 1, if polygon edge is vertical
            if (tmp_strt->x == tmp_end->x) {
                tmp_y = m_bisector * tmp_strt->x + c_bisector;
                idx_edge = current_edge->idx_edge;

                // Whether the y-intersection is in the range
                vrtln_check(&tmp_strt, &tmp_end, tmp_y, itct_ps, &count, idx_edge);
            }

            // Subcondition 2, if polygon edge is horizontal
            else if (tmp_strt-> y == tmp_end->y) {
                tmp_x = (tmp_strt->y - c_bisector) / m_bisector;
                idx_edge = current_edge->idx_edge;

                // Whether the x-intersection is in the domain
                hrzln_check(&tmp_strt, &tmp_end, tmp_x, itct_ps, &count, idx_edge);
            }

            // Subcondition 3, other than the gradient of edge is zero or infinity
            else {
                // Gradient and c value of polygon edge
                m_pgedge = gradient(tmp_strt, tmp_end);
                c_pgedge = tmp_end->y - m_pgedge * tmp_end->x;

                // If two lines are not parallel
                if (m_bisector != m_pgedge) {
                    tmp_x = (c_pgedge - c_bisector) / (m_bisector - m_pgedge);
                    tmp_y = m_bisector * tmp_x + c_bisector;

                    // If intersection point is in the domain and range
                    if ( domain_check(&tmp_strt, &tmp_end, &tmp_x) 
                            && range_check(&tmp_strt, &tmp_end, &tmp_y) ) {
                        // Special condition may occur when intersect the end
                        // points of two connected edge
                        if (count > 0) {
                            if ((*itct_ps)[count-1].x != tmp_x 
                                && (*itct_ps)[count-1].y != tmp_y) {
                                    (*itct_ps)[count].x = tmp_x;
                                    (*itct_ps)[count].y = tmp_y;
                                    (*itct_ps)[count].idx_edge = current_edge->idx_edge;
                                    count++;
                            }
                        } 
                        else {
                            (*itct_ps)[count].x = tmp_x;
                            (*itct_ps)[count].y = tmp_y;
                            (*itct_ps)[count].idx_edge = current_edge->idx_edge;
                            count++;
                        }
                    }
                }   
            }

            current_edge = current_edge->next_edge;
            printf("current_edge = %p\n", current_edge);  
        }
    }
}

/******************************************************************************/
/* 
    gradient() functions:
    Inputs: 
        pointer to point1, pointer to point2
    Outputs: 
        Returning the gradient of bisector
*/

double 
gradient(point_t *p1, point_t *p2) {
    return (p2->y - p1->y) / (p2->x - p1->x);

}

/******************************************************************************/
/*
    vrtln_check() function:
    Description:
        This function is used for checking whether the y-value of intersection 
        point is inside the range.
    Inputs:
        pointer to start point
        pointer to end point
        y value of intersection point
        intersection points list
        number of recorded points
        index of corresponding edge
    Outputs:
        Update the inforamtion of start/end point into intersection points list
*/
void 
vrtln_check(point_ptr *tmp_strt, point_ptr *tmp_end, double itct_y, 
            itct_p_t** itct_ps, int *count, int idx_edge){

    if ( range_check(tmp_strt, tmp_end, &itct_y) ) {

        (*itct_ps + *count)->x = (*tmp_strt)->x;
        (*itct_ps + *count)->y = itct_y;
        (*itct_ps + *count)->idx_edge = idx_edge;
        (*count)++; 
    }
}

/******************************************************************************/
/*
    hrzln_check() function:
    Description:
        This function is used for checking whether x-value of intersection 
        point is inside the domain.
    Inputs:
        pointer to start point
        pointer to end point
        x value of intersection point
        intersection points list
        number of recorded points
        index of corresponding edge
    Outputs:
        Update the inforamtion of start/end point into intersection points list
*/
void 
hrzln_check(point_ptr *tmp_strt, point_ptr *tmp_end, double itct_x, 
            itct_p_t** itct_ps, int *count, int idx_edge){

    if ( domain_check(tmp_strt, tmp_end, &itct_x) ) {

        (*itct_ps + *count)->x = itct_x;
        (*itct_ps + *count)->y = (*tmp_strt)->y;
        (*itct_ps + *count)->idx_edge = idx_edge;
        (*count)++;
    }
}

/******************************************************************************/
/*
    domain_check() function:
    Description: 
        This function is used to check whether the x value of intersection point 
        positions between the start point and end point.
    Inputs:
        pointer to start point
        pointer to end point
        x value of intersection poin
    Outputs:
        Return TRUE(1) if it is in the domain
        Return FALSE(0) if it is not in the domain
        
*/
int 
domain_check(point_ptr *tmp_strt, point_ptr *tmp_end, double *itct_x){

    if ((float)(*tmp_strt)->x == (float)*itct_x) {
        *itct_x = (*tmp_strt)->x;
    }
    else if ((float)(*tmp_end)->x == (float)*itct_x) {
        *itct_x = (*tmp_end)->x;
    }
    
    if ( ((*tmp_strt)->x <= *itct_x && *itct_x <= (*tmp_end)->x) || 
            ((*tmp_strt)->x >= *itct_x && *itct_x >= (*tmp_end)->x) ){
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************/
/*
    range_check() function:
    Description:
        This function is used to check whether the y value of intersection point 
        positions between the start point and end point.
    Inputs:
        pointer to start point
        pointer to end point
        y value of intersection poin
    Outputs:
        Return TRUE(1) if it is in the range
        Return FALSE(0) if it is not in the range
        
*/
int 
range_check(point_ptr *tmp_strt, point_ptr *tmp_end, double *itct_y){

    if ((float)(*tmp_strt)->y == (float)*itct_y) {
        *itct_y = (*tmp_strt)->y;
    }
    else if ((float)(*tmp_end)->y == (float)*itct_y) {
        *itct_y = (*tmp_end)->y;
    }
    
    if ( ((*tmp_strt)->y <= *itct_y && *itct_y <= (*tmp_end)->y) || 
            ((*tmp_strt)->y >= *itct_y && *itct_y >= (*tmp_end)->y) ){
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************/
/*
    tower_alloc_chk() function:
    Description:
        This function is used to check if the desired tower is inside the 
        desired face/voronoi cell.
    Inputs:
        pointer to the first edge of the face/ voronoi cell
        pointer to the tower
        pointer to vertics list
    Outputs:
        Returning True means insides the edge, Flase means outside the edge
        
        
*/
int 
tower_alloc_chk(edge_ptr* fst_edge, tower_ptr* tower, point_ptr** vertcs) {
    
    edge_ptr current_edge = *fst_edge;
    int t = 0;
    double m, c;

    while (current_edge != *fst_edge || t == 0) {
        
        point_t strt_p, end_p;
        strt_p.x = (*vertcs)[current_edge->idx_strt]->x;
        strt_p.y = (*vertcs)[current_edge->idx_strt]->y;
        end_p.x = (*vertcs)[current_edge->idx_end]->x;
        end_p.y = (*vertcs)[current_edge->idx_end]->y;

        if ( strt_p.x == end_p.x ){
            // what if strt_p.x == end_strt_p.x
            if (strt_p.y <= strt_p.y) {
                // if str_y < end_p.y, line from bottom to top vertically
                if ( (*tower)->x >= strt_p.x ) {
                    t++;
                }
            }
            else {
                // if str_y > end_p.y, from top to bottom vertically
                if ( (*tower)->x <= strt_p.x ) {
                    t++;
                }
            }
        }
        else if ( strt_p.x > end_p.x ) {
            // what if strt_p.x > end_strt_p.x
            m = gradient(&strt_p, &end_p);
            c = end_p.y - m * end_p.x;
            if (yr_test_above((*tower)->x, (*tower)->y, m, c) == TRUE) { // Condition true if above
                t++;
            }
            else {
                return FALSE;
            }

        }
        else if ( strt_p.x < end_p.x ){
            // what if strt_p.x < end_strt_p.x
            m = gradient(&strt_p, &end_p);
            c = end_p.y - m * end_p.x;
            if (yr_test_below((*tower)->x, (*tower)->y, m, c) == TRUE) { // Condition true if below
                t++;
            }
            else {
                return FALSE;
            }
        }

        current_edge = current_edge->next_edge;
    }

    return TRUE;
}

/******************************************************************************/
/*
    yr_test_above() function:
    Description:
        
    Inputs:
        
    Outputs:
        Returning True(1) if above
        
        
*/
int 
yr_test_above(double x, double y, double m, double c) {
    int result = FALSE; 
    return result+=((y - (m * x + c)) >= 0);
}

/******************************************************************************/
/*
    yr_test_below() function:
    Description:
        
    Inputs:
        
    Outputs:
        Returning True(1) if below
        
        
*/
int 
yr_test_below(double x, double y, double m, double c) {
    int result = FALSE; 
    return result+=((y - (m * x + c)) <= 0);
}

/******************************************************************************/
/*
    vorocells_update() function:
    Description:   
        This function is used to update the vorocells list
    Inputs:
        
    Outputs:
        Update the info of towers is positioned in which face
*/

void 
vorocells_update(   int num_vcells, vorocells_ptr** vorocells, 
                    int num_dcel, edge_ptr **dcel, 
                    int num_tower, tower_ptr** towers,
                    point_ptr** vertics){
    int v1, v2, idx_t1;
    edge_ptr fst_edge1, fst_edge2, tmp_edge;

    for (v1 = 0; v1 < num_tower; v1++) {

        idx_t1 = (*vorocells)[v1]->idx_tower; // index of tower 1
        fst_edge1 = (*vorocells)[v1]->fst_edge; // pointer to fitst cell edge

        // If the vth tower is located in vth voronoi cell
        if (tower_alloc_chk(&fst_edge1, &(*towers)[idx_t1], vertics)) {
            continue;
        }
        else { // If not, checking the rest of voronoi cells
            for (v2 = v1 + 1; v2 < num_vcells; v2++) {

                fst_edge2 = (*vorocells)[v2]->fst_edge; // pointer second cell edge

                // Find the corresponding cells
                if (tower_alloc_chk(&fst_edge2, &(*towers)[idx_t1], vertics)) {

                    corrf_update(&((*vorocells)[v1]->fst_edge), v2);
                    corrf_update(&((*vorocells)[v2]->fst_edge), v1);
                    
                    // Swapping the fst_edge of corresponding face
                    tmp_edge = fst_edge1;
                    (*vorocells)[v1]->fst_edge = fst_edge2;
                    (*vorocells)[v2]->fst_edge = tmp_edge;

                    break;
                }
            }
        }
    }
    

}

/******************************************************************************/
/*
    tf_find() function:
    Description:   
        This function is used to find which face the givein tower is located in
    Inputs:
        pointer to vorocells list
        number of faces
        the index of tower
    Outputs:
        returning an integer which represents the nth faces it locates in
*/
int 
tf_find(vorocells_ptr** vorocells, int num_vcells, int idx_tower){
    int i;

    for (i = 0; i < num_vcells; i++){
        if ((*vorocells)[i]->idx_tower == idx_tower){
            break;
        }
    }

    return i;
}

/******************************************************************************/
/*
    end_edge_find() function:
    Description:   
        This function is used to find the end edge with desired intersection point
        as starting point (points to its twin edge)
    Inputs:
        pointer to intersection point
        pointer to tmporary edge
        pointer to vertices list
    Outputs:
        
*/

void 
end_edge_find(itct_p_t *itct_ps, edge_ptr *tmp_edge, point_ptr **vertics){

    if ((*vertics)[(*tmp_edge)->idx_strt]->x != (*itct_ps).x 
            && (*vertics)[(*tmp_edge)->idx_strt]->y != (*itct_ps).y) {
                        
            (*tmp_edge) = (*tmp_edge)->otr_edge;
    }
}

/******************************************************************************/
/*
    strt_edge_find() function:
    Description:   
        This function is used to find the start edge with desired intersection point
        as ending point (points to its twin edge)
    Inputs:
        pointer to intersection point
        pointer to tmporary edge
        pointer to vertices list
    Outputs:
        
*/

void 
strt_edge_find(itct_p_t *itct_ps, edge_ptr *tmp_edge, point_ptr **vertics){

    if ((*vertics)[(*tmp_edge)->idx_end]->x != (*itct_ps).x 
            && (*vertics)[(*tmp_edge)->idx_end]->y != (*itct_ps).y) {
                        
            (*tmp_edge) = (*tmp_edge)->otr_edge;
    }
}

/******************************************************************************/
/*
    vertics_check() function:
    Description:   
        This function is used to find if a vertices already allocates in vertics
        list.
    Inputs:
        pointer to intersection point
        number of vertices
        pointer to vertics list
    Outputs:
        Returning TRUE if duplicated points are found, FALSE as not found
*/

int
vertics_check(itct_p_t itct_p, int num_vertics, point_ptr **vertics, int *nth) {
    int i;

    for (i = 0; i < num_vertics; i++) {
        if ( (*vertics)[i]->x == itct_p.x && (*vertics)[i]->y == itct_p.y) {
            *nth = i;
            return TRUE;
        }
    }
    return FALSE;
}

/******************************************************************************/
/*
    itct_pivrt() function:
    Description:   
        This function is used to inverse the position of intersection points.
    Inputs:
        pointer to intersection point
    Outputs:
        Inversing the position of intersection points.
*/

void 
itct_pivrt(itct_p_t **itct_ps){
    itct_p_t tmp_itct;

    tmp_itct = (*itct_ps)[1];
    (*itct_ps)[1] = (*itct_ps)[0];
    (*itct_ps)[0] = tmp_itct;
}

/******************************************************************************/
/*
    dcel_find() function:
    Description:
        This function is used to find the nth postion of the DCE in DCEL
    Inputs:
        index of start point
        index of end point
        number of dcel
        pointer to dcel
    Outputs:
        Returning the number of poisiont in DCEL
*/
int
dcel_find(int idx_strt_p, int idx_end_p, int num_dcel, edge_ptr **dcel) {
    int i;

    for (i = 0; i < num_dcel; i++) {
        if (idx_strt_p == (*dcel)[i]->idx_strt 
                && idx_end_p == (*dcel)[i]->idx_end) {
            return i;
        }
    }

    printf("DCEL_Find Failure!");
    exit(EXIT_FAILURE);
}

/******************************************************************************/
/*
    vertex_find() function:
    Description: 
        This function is used to find the index of vertex in vertices list
    Inputs;
        number of vertices
        pointer to vertices list
        pointer to intersection point
    Outputs:
        Returning the index of vertex
*/

int 
vertex_find(int num_vertics, point_ptr** vertics, itct_p_t itct_p){
    int i;

    for (i = 0; i < num_vertics; i++) {
        if ( (*vertics)[i]->x == itct_p.x && (*vertics)[i]->y == itct_p.y) {
            return i;
        }
    }

    printf("Error in vertex_find funcion!\n");
    exit(EXIT_FAILURE);
}

/******************************************************************************/
/*
    corrf_update() function;
    Description:
        This funciton is used to change the number of corrsponding face of edges
        with desired number
    Inputs:
        pointer to the first edge of the face
        the numbe of nth face
    Outputs:
        Rewriting the coresponding face into nth
*/

void 
corrf_update(edge_ptr* fst_edge, int nth){
    int t = FALSE;
    edge_ptr current_edge = *fst_edge;
    
    while (current_edge != *fst_edge || t != TRUE) {
        current_edge->corr_face = nth;

        current_edge = current_edge->next_edge;
        t = TRUE;
    }

}

/******************************************************************************/
/*
    bounded_test() function;
    Description:
         This function is used to test the edge whether it is bounded or 
         unbounded edge
    Inputs:
        pointer to testing edge
        condition integer used to output the result
    Outputs:
        *bd = TRUE if bounded
        *bd = FALSE if unbounded
*/

void 
bounded_test(edge_ptr *edge, int * bd){

    if ( (*edge)->next_edge->otr_edge != NULL
            && (*edge)->prev_edge->otr_edge != NULL ) {
        // Bounded edge is found
        *bd = TRUE;
    }
    else {
        // Unbounded edge is found
        *bd = FALSE;
    }
    
}

/******************************************************************************/
/*
    cycle_chk() function:
    Description:
        This function is uesed to check if prev_edge and next_edge can not loop
        back to current edge.
    Inputs:
        pointer to edge
    Outputs:
        Exit the program if infinity loop meet.

*/
void 
cycle_chk(edge_ptr *fst_edge){
    int t = 0;
    edge_ptr current_edge;
    current_edge = *fst_edge;

    while (current_edge->next_edge != *fst_edge) {
        current_edge = current_edge->next_edge;
        t++;

        if (t >100) {
            printf("Infinity loop!\n");
            printf("Error in next_edge!\n");
            exit(EXIT_FAILURE);
        }
    }

    current_edge = *fst_edge;
    t=0;
    while (current_edge->prev_edge != *fst_edge) {
        current_edge = current_edge->prev_edge;
        t++;

        if (t >100) {
            printf("Infinity loop!\n");
            printf("Error in prev_edge!\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Cycle Check Completed!\n");
}

/******************************************************************************/
/*
    edge_elimination() function:
    Description:
        This function is uesed to eliminate edges.
    Inputs:
        number of corresponding face
        pointer to number of DCEL
        pointer to DCEL
    Outputs:
        Eliminating edges in DCEL

*/

void 
edge_elimination(int corr_face, int *num_dcel, edge_ptr **dcel) {
    int i;

    for (i = 0; i < *num_dcel; i++) {
        if ( (*dcel)[i]->corr_face == corr_face ) {
            (*dcel)[i]->idx_strt = 9999; 
            (*dcel)[i]->idx_end = 9999; 
            (*dcel)[i]->prev_edge = NULL; 
            (*dcel)[i]->next_edge = NULL; 
            (*dcel)[i]->otr_edge = NULL; 
            (*dcel)[i]->corr_face = 9999; 
            (*dcel)[i]->idx_edge = 9999; 
        }
        else if ((*dcel)[i]->otr_edge != NULL) {
            if ((*dcel)[i]->otr_edge->corr_face == corr_face) {
                (*dcel)[i]->idx_strt = 9999; 
                (*dcel)[i]->idx_end = 9999; 
                (*dcel)[i]->prev_edge = NULL; 
                (*dcel)[i]->next_edge = NULL; 
                (*dcel)[i]->otr_edge = NULL; 
                (*dcel)[i]->corr_face = 9999; 
                (*dcel)[i]->idx_edge = 9999; 
            }
        }
        else {
            continue;
        }
    }

}

/******************************************************************************/
/*
    ubd_edge_update() function:
    Description:
        This function is update the cell with unbounded edge.
    Inputs:
        
    Outputs:
        

*/
void
ubd_edge_update(edge_ptr *nwform_edge, edge_ptr *crnt_edge, edge_ptr *itct_edge) {
    edge_ptr tmp_next_edge;

    // Updating crnt_edge and its next edge
    tmp_next_edge = (*crnt_edge)->next_edge;//9-6, 4-8

    if (tmp_next_edge->idx_end == (*itct_edge)->idx_strt) {
        (*crnt_edge)->idx_end = (*nwform_edge)->idx_end;
        (*crnt_edge)->next_edge = (*nwform_edge)->next_edge;
        (*crnt_edge)->next_edge->prev_edge = (*crnt_edge);
        (*nwform_edge) = (*crnt_edge)->next_edge;
    }
    else {
        (*crnt_edge)->next_edge = *nwform_edge;//9-11
        (*nwform_edge)->prev_edge = (*crnt_edge); //8-9, 2-4
    }
    

    while ( (*nwform_edge)->next_edge != tmp_next_edge->otr_edge){// != 6-9, 8-4
        (*nwform_edge)->corr_face = (*crnt_edge)->corr_face;
        (*nwform_edge) = (*nwform_edge)->next_edge;
    }

    // 11-6, 10-8
    if ((*nwform_edge)->idx_end != (*itct_edge)->idx_strt) {

        // Updating edge 6-2
        tmp_next_edge->next_edge->prev_edge = (*nwform_edge)->prev_edge;
        tmp_next_edge->next_edge->idx_strt = (*nwform_edge)->idx_strt;
        // Updating edge 9-11
        (*nwform_edge)->prev_edge->next_edge = tmp_next_edge->next_edge;//9-11
        
    }
    else {
        // Looping back to the start of bisector
        (*nwform_edge)->corr_face = (*crnt_edge)->corr_face;
        (*nwform_edge)->next_edge = tmp_next_edge->next_edge;
        (*nwform_edge)->prev_edge->next_edge = (*nwform_edge);
        tmp_next_edge->next_edge->prev_edge = (*nwform_edge);
    }
    
}

/******************************************************************************/
/*
    diametoer_calc() function:
    Description: 
        This function is used to output the diameter of each voronocell into
        output file
    Intpus:

    Outpus: 
        Writing diametor data into outoupt file
*/

void 
diameter_calc(FILE **output_file, int num_vcells, vorocells_ptr **vorocells, 
                tower_ptr **towers, point_ptr **vertics, info_ptr **info_array) {
    int i, idx_end_p, fst_idx_p;
    int t1= 0, t2 = 0;
    double distn, tmp_distn;
    edge_ptr fst_edge, tmp_edge1, tmp_edge2;

    for (i = 0; i < num_vcells; i++) {
        printf("when i = %d\n", i);
        fst_edge = (*vorocells)[i]->fst_edge;
        tmp_edge1 = fst_edge;
        tmp_edge2 = tmp_edge1;

        distn = 0;
        t1 = 0;

        while (tmp_edge1 != fst_edge || t1 == 0 ){
            fst_idx_p = tmp_edge1->idx_strt;
            idx_end_p = tmp_edge1->idx_end;
            printf("fst_idx_p = %d, idx_end_p = %d\n", fst_idx_p, idx_end_p);
            t2 = 0;

            while (idx_end_p != fst_idx_p || t2==0) {
                printf("t2 = %d, idx_end_p = %d\n", t2, idx_end_p);
                tmp_distn = distance_calc((*vertics)[fst_idx_p], 
                                            (*vertics)[idx_end_p]);

                if (tmp_distn > distn) {
                    distn = tmp_distn;
                }

                tmp_edge2 = tmp_edge2->next_edge;
                idx_end_p = tmp_edge2->idx_end;
                
                t2++;

                if (t2 > 10) {
                    printf("error in calc!\n");
                    exit(EXIT_FAILURE);
                }
            }

            tmp_edge1 = tmp_edge1->next_edge;
            tmp_edge2 = tmp_edge1;
            t1++;

            
        }

        fprintf(*output_file, "Watchtower ID: %s, Postcode: %s, Population Served: %d, "
                "Watchtower Point of Contact Name: %s, x: %f, y: %f, "
                "Diameter of Cell: %f\n", 
                (*towers)[i]->ID, 
                (*towers)[i]->postcode,
                (*towers)[i]->pop,
                (*towers)[i]->mang,
                (*towers)[i]->x, 
                (*towers)[i]->y, 
                distn);

        (*info_array)[i]->distn = distn;

    }
}

/******************************************************************************/
/*
    distance_cale() function:
    Description: 
        This function is used to calculate the distance between two point
    Intputs:
        pointer to point of tower
        pointer to vertex
    Output: 
        Returning the distance
*/

double
distance_calc(point_ptr tower_p, point_ptr vertex_p){
    double x1, x2;
    double y1, y2;

    x1 = tower_p->x;
    x2 = vertex_p->x;
    y1 = tower_p->y;
    y2 = vertex_p->y; 

    printf("In distance_calc: 1,(%f, %f) 2,(%f, %f)\n", x1, y1, x2, y2);

    return sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) );
}
/***********************************END****************************************/
