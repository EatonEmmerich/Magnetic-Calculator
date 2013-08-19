#include <stdio.h>
#include <stdlib.h>
#include "DataTypes.h"
#include "CoordinatesCalculator.h"

vect_list * create_calc_position( double xl, double yl, double zl, double xh, double yh, double zh, double xi, double yi, double zi){
    vect_list * position = NULL;
    vect_list * position_start = NULL;
    double xtmp, ytmp, ztmp;
    printf("\nUsing cubic position matrix with:");
    printf("\nlow x = %lf",xl);
    printf("\nlow y = %lf",yl);
    printf("\nlow z = %lf",zl);
    printf("\nhigh x = %lf",xh);
    printf("\nhigh y = %lf",yh);
    printf("\nhigh z = %lf",zh);
    printf("\nx intervals = %lf",xi);
    printf("\ny intervals = %lf",yi);
    printf("\nz intervals = %lf",zi);
    xtmp = xh;
    while(xtmp > xl){
        ytmp = yh;
        while(ytmp > yl){
            ztmp = zh;
            while(ztmp > zl){
                add_vector_val(&position_start, &position, xtmp, ytmp, ztmp);
                if(ztmp - zi <= zl){
                    ztmp = zl;
                    add_vector_val(&position_start, &position, xtmp, ytmp, ztmp);
                }
                else{
                    ztmp = ztmp - zi;
                }
            }
            if(ytmp - yi <= yl){
                ytmp = yl;
                add_vector_val(&position_start, &position, xtmp, ytmp, ztmp);
            }
            else{
                ytmp = ytmp - yi;
            }
        }
        if(xtmp - xi <= xl){
            xtmp = xl;
            add_vector_val(&position_start, &position, xtmp, ytmp, ztmp);
        }
        else{
            xtmp = xtmp - xi;
        }
    }
    return position_start;
}

vect_list * bounded_position_interval(position_vector* bounds, double xi, double yi, double zi){
    double highx;
    double lowx;
    double highy;
    double lowy;
    double highz;
    double lowz;
    get_high_low(bounds,&highx, &lowx, &highy, &lowy, &highz, &lowz);
    printf("INTERVAL");
    return create_calc_position(lowx, lowy, lowz, highx, highy, highz, xi, yi, zi);
}
vect_list * bounded_position(position_vector* bounds){
    double highx;
    double lowx;
    double highy;
    double lowy;
    double highz;
    double lowz;
    double xi, yi, zi;
    get_high_low(bounds,&highx, &lowx, &highy, &lowy, &highz, &lowz);
    xi = ((highx-lowx)/10);
    yi = ((highy-lowy)/10);
    zi = ((highz-lowz)/10);
    return create_calc_position(lowx, lowy, lowz, highx, highy, highz, xi, yi, zi);
}

int get_high_low(position_vector* bounds, double* highx, double* lowx, double* highy, double* lowy, double* highz, double* lowz){
    *highx = bounds->position->x;
    *lowx = bounds->position->x;
    *highy = bounds->position->y;
    *lowy = bounds->position->y;
    *highz = bounds->position->z;
    *lowz = bounds->position->z;
    bounds = bounds->next;
    while(bounds != NULL){
            if(bounds->position->x > *highx) *highx = bounds->position->x;
            else if(bounds->position->x < *lowx) *lowx = bounds->position->x;
            if(bounds->position->y > *highy) *highy = bounds->position->y;
            else if(bounds->position->y < *lowy) *lowy = bounds->position->y;
            if(bounds->position->z > *highz) *highz = bounds->position->z;
            else if(bounds->position->z < *lowz) *lowz = bounds->position->z;
            bounds = bounds->next;
    }
    return 0;
}

int get_high_low_list(vect_list* bounds, double* highx, double* lowx, double* highy, double* lowy, double* highz, double* lowz)
{
    *highx = bounds->vector_element->x;
    *lowx = bounds->vector_element->x;
    *highy = bounds->vector_element->y;
    *lowy = bounds->vector_element->y;
    *highz = bounds->vector_element->z;
    *lowz = bounds->vector_element->z;
    bounds = bounds->next;
    while(bounds != NULL){
            if(bounds->vector_element->x > *highx) *highx = bounds->vector_element->x;
            else if(bounds->vector_element->x < *lowx) *lowx = bounds->vector_element->x;
            if(bounds->vector_element->y > *highy) *highy = bounds->vector_element->y;
            else if(bounds->vector_element->y < *lowy) *lowy = bounds->vector_element->y;
            if(bounds->vector_element->z > *highz) *highz = bounds->vector_element->z;
            else if(bounds->vector_element->z < *lowz) *lowz = bounds->vector_element->z;
            bounds = bounds->next;
    }
    return 0;
}

int transpose_problem(vect_list ** position, position_vector** j_field){
    double xh, xl, yh, yl, zh, zl;
    double xi, yi, zi;
    int p0 = 0;
    vect_list * position_start = *position;
    position_vector * j_field_start = *j_field;

    while(*position != NULL){
        if(((*position)->vector_element->x == 0) && ((*position)->vector_element->y == 0) && ((*position)->vector_element->z == 0))
        {
            p0 = 1;
        }
        *position = (*position)->next;
    }
    if(p0 == 1){
        *position = position_start;
        get_high_low_list(*position,&xh,&xl,&yh,&yl,&zh,&zl);
        xi = xh - xl;
        yi = yh - yl;
        zi = zh - zl;
        while(*position != NULL){
            (*position)->vector_element->x = (*position)->vector_element->x + xi;
            (*position)->vector_element->y = (*position)->vector_element->y + yi;
            (*position)->vector_element->z = (*position)->vector_element->z + zi;
            *position = (*position)->next;
        }
        *position = position_start;
        while(*j_field != NULL){
            (*j_field)->position->x = (*j_field)->position->x + xi;
            (*j_field)->position->y = (*j_field)->position->y + yi;
            (*j_field)->position->z = (*j_field)->position->z + zi;
            *j_field = (*j_field)->next;
        }
        *j_field = j_field_start;
    }
    return p0;
}
