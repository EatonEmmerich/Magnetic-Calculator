#include<stdlib.h>
#include<stdio.h>
#include"DataTypes.h"
#include<math.h>
#include"JToB.h"

/*
% Essentially, what we have to do for segments with arbitrary direction is
% the following. First, we have to rotate the coordinate system such that
% the segment is pointing along the x direction in the new (primed)
% coordinate system. Then we compute the contribution to the field, which,
% because it's a vector, we have to rotate back into the original
% coordinate system.

calc_position {x,y,z} coordinates for the magnetic field to be calculated.
current density {pos: x,y,z;    coordinates for the current field.
                 curr:x,y,z}    vector values for the current at specified coordinates.
dimensions    {x,y,z} (Width, heigth, length) of current segments.
*/
position_vector * calculateBfield(vect_list* calc_position, position_vector * current_density, vect_list * dimensions){
    position_vector * b_start = NULL;
    position_vector * b = NULL;        //in this case current_density is used as the magnetic field density.
    position_vector * b_temp;
    vect_list * calc_position_start = calc_position;
    vect_list * vp_start = NULL;
    vect_list * vp = NULL;
    vect_list * ki;
    vect_list * kistart;
    double yoverx, zoverr, a1, a2, a3;
    vect * tempv0;
    vect * tempv;

    halfofvect(&dimensions);//take half of dimensions to move to the edges of the segments

    //Initialize B field to zero, should be same length as current_density.
    printf("\nStarting calculation process.");
    printf("\nInitializing B field to zero");
    while(calc_position != NULL){
        b_temp = (position_vector*)malloc(sizeof(position_vector));
        b_temp->current_density = (vect*)malloc(sizeof(vect));
        b_temp->position = (vect*)malloc(sizeof(vect));
        if(b_temp == NULL){
            printf("out of memory error");
        }
        else{
            if(b_temp->current_density == NULL || b_temp->position == NULL){
                printf("out of memory error");
            }
        }
        b_temp->current_density->x = 0.00;
        b_temp->current_density->y = 0.00;
        b_temp->current_density->z = 0.00;
        b_temp->position->x = calc_position->vector_element->x;
        b_temp->position->y = calc_position->vector_element->y;
        b_temp->position->z = calc_position->vector_element->z;
        b_temp->next = NULL;
        add_position_vector_pointer(&b_start,&b,b_temp);
        printf("\nadded\n");
        /*if(b_start == NULL){
            b = b_temp;
            b_start = b;
        }
        else{
            b->next = b_temp;
            b = b->next;
        }*/

        calc_position = calc_position->next;
        printf(".\n");
    }
    b = b_start;
    calc_position = calc_position_start;
    printf("    ... Done\nCalculating Magnetic field:");

    while(current_density != NULL){
       /* printf("start whileloop");
        scanf("%d",&inp);*/

        //printf("\nassign values to temporary variables.");
        tempv0 = (vect *)malloc(sizeof(vect));
        /*printf("malloced tempv0");
        scanf("%d",&inp);*/
        //obtain spherical coordinates for current segment current value.
        //vect * spheric = cartesianToSpherical(current_density->current_density);
        //obtain yoverx and zoverr for usage for determining primed positions. also state values a1 a2 and a3 for use.
        zoverr = current_density->current_density->z/sqrt((current_density->current_density->x)*(current_density->current_density->x) +
                   (current_density->current_density->y)*(current_density->current_density->y) +
                   (current_density->current_density->z)*(current_density->current_density->z));
        a3 = (yoverx/sqrt(yoverx*yoverx+1));
        if(current_density->current_density->x != 0){
            yoverx = current_density->current_density->y/current_density->current_density->x;


            a1 = 1/sqrt(yoverx*yoverx+1);
            a2 = (sqrt(1-zoverr*zoverr));
        }
        else{
            a1 = 0;
            a2 = 1;
            yoverx = 0;
            if((current_density->current_density->x == 0) && (current_density->current_density->y == 0) && (current_density->current_density->z == 0)){
                zoverr = 0;
                a3 = 1;
            }
        }
        //printf(".\n");
        //printf("calculate offsets for rotation matrix");

        tempv0->x = (a1)*(a2)*current_density->position->x -
                    ((a3)*a2*current_density->position->y) -
                    ((zoverr)*current_density->position->z);//obtain offset to subtract from calculated value.
        tempv0->y = -(a3)*current_density->position->x +
                    (a1)*current_density->position->y;
        tempv0->z = (a1)*(zoverr)*current_density->position->x +
                    (a3)*(zoverr)*current_density->position->y +
                    (a2)*current_density->position->z;
        //printf(".\n");
        //printf("Multiply with rotation matrix");
        while(calc_position != NULL){                   //multiply R with VP and subtract V0
            tempv = (vect *)malloc(sizeof(vect));
            tempv->x = (a1)*(a2)*calc_position->vector_element->x -
                    ((a3)*a2*calc_position->vector_element->y) -
                    ((zoverr)*calc_position->vector_element->z) - tempv0->x;
            tempv->y = -(a3)*calc_position->vector_element->x +
                    (a1)*calc_position->vector_element->y - tempv0->y;
            tempv->z = (a1)*(zoverr)*calc_position->vector_element->x +
                    (a3)*(zoverr)*calc_position->vector_element->y +
                    (a2)*calc_position->vector_element->z - tempv0->z;
            add_vector(&vp_start,&vp,tempv);
            calc_position = calc_position->next;
        }
        calc_position = calc_position_start;
        /*printf("malloced vp_start");
        scanf("%d",&inp);*/
        free(tempv0);
        /*printf("freed tempv0");
        scanf("%d",&inp);*/
        //printf(".\n");
        //printf("calculate the integral values");
        ki = kicalc(vp_start,dimensions->vector_element);           //calculate Ki
       /* printf("malloced kistart");
        scanf("%d",&inp);*/
        //printf(".\n");
        kistart = ki;
        //printf("free vp vector list");
        free_vect_list(&vp_start);
       /* printf("freed vpstart");
        scanf("%d",&inp);*/
        vp_start = NULL;
        //printf(".\n");
        //printf("Times ki matrix with inverting R matrix");
        while(ki != NULL){                          //times Ki with inverting R matrix to rotate to original coordinates
            ki->vector_element->x = a1*a2*ki->vector_element->x -
                                a3*ki->vector_element->y +
                                a1*a2*ki->vector_element->z;
            ki->vector_element->y = - a3*a2*ki->vector_element->x +
                                a1*ki->vector_element->y +
                                a3*a2*ki->vector_element->z;
            ki->vector_element->z = - a2*ki->vector_element->x +
                                a2*ki->vector_element->z;
            ki = ki->next;
        }
        //printf(".\n");
        ki = kistart;
        //  % Cross the result with the original current density to give the field
        //  % contribution:
        //printf("Cross result with curent density");
        while(ki != NULL){
            b->current_density->x = b->current_density->x +  (current_density->current_density->y*ki->vector_element->z -
                                                              ki->vector_element->y*current_density->current_density->z);
            b->current_density->y = b->current_density->y + (-current_density->current_density->x*ki->vector_element->z +
                                                              ki->vector_element->x*current_density->current_density->z);
            b->current_density->z = b->current_density->z +  (current_density->current_density->x*ki->vector_element->y -
                                                              ki->vector_element->x*current_density->current_density->y);
            ki = ki->next;
            b = b->next;
        }
        ki = kistart;
        //printf(".\n");
        //printf("free vect list KI");
        free_vect_list(&kistart);
        kistart = NULL;
        /*printf("freed kistart");
        scanf("%d",&inp);*/
        //printf(".\n");
        b = b_start;
        current_density = current_density->next;
        dimensions = dimensions->next;
    }

    position_vector_divten(&b_start);
    return b_start;
}

/*
 * Calculate the value Ki.
 *
 */
 vect_list * kicalc(vect_list * pin, vect * dimensions){
     vect_list * ki = NULL;
     vect_list * ki_start = NULL;
     vect * curr;
     vect * temp2;
     vect * temp = NULL;
     //char isfirst = 0;
     temp = (vect*)malloc(sizeof(vect));
     //printf("kicalc");
     if(temp == NULL){
        printf("/nnot enough memory error.");
     }
     while(pin != NULL){

        //printf("while");
        temp->x = (pin->vector_element->x+dimensions->x);
        temp->y = (pin->vector_element->y+dimensions->y);
        temp->z = (pin->vector_element->z+dimensions->z);//indefInt(x+a,y+b,z+c)
        curr = indefintegral(temp);
        temp->x = (pin->vector_element->x-dimensions->x);
        temp->y = (pin->vector_element->y+dimensions->y);// - indefInt(x-a,y+b,z+c)
        temp->z = (pin->vector_element->z+dimensions->z);
        temp2 = indefintegral(temp);
        free(temp);
        temp = temp2;
        curr->x = curr->x - temp->x;
        curr->y = curr->y - temp->y;
        curr->z = curr->z - temp->z;
        temp->x = (pin->vector_element->x+dimensions->x);
        temp->y = (pin->vector_element->y-dimensions->y);// - indefInt(x+a,y-b,z+c)
        temp->z = (pin->vector_element->z+dimensions->z);
        temp2 = indefintegral(temp);
        free(temp);
        temp = temp2;
        curr->x = curr->x - temp->x;
        curr->y = curr->y - temp->y;
        curr->z = curr->z - temp->z;
        temp->x = (pin->vector_element->x-dimensions->x);
        temp->y = (pin->vector_element->y-dimensions->y);// + indefInt(x+a,y+b,z-c)
        temp->z = (pin->vector_element->z+dimensions->z);
        temp2 = indefintegral(temp);
        free(temp);
        temp = temp2;
        //printf("mid");
        curr->x = curr->x + temp->x;
        curr->y = curr->y + temp->y;
        curr->z = curr->z + temp->z;
        temp->x = (pin->vector_element->x+dimensions->x);
        temp->y = (pin->vector_element->y+dimensions->y);// - indefInt(x+a,y+b,z-c)
        temp->z = (pin->vector_element->z-dimensions->z);
        temp2 = indefintegral(temp);
        free(temp);
        temp = temp2;
        curr->x = curr->x - temp->x;
        curr->y = curr->y - temp->y;
        curr->z = curr->z - temp->z;
        temp->x = (pin->vector_element->x-dimensions->x);
        temp->y = (pin->vector_element->y+dimensions->y);// + indefInt(x-a,y+b,z-c)
        temp->z = (pin->vector_element->z-dimensions->z);
        temp2 = indefintegral(temp);
        free(temp);
        temp = temp2;
        curr->x = curr->x + temp->x;
        curr->y = curr->y + temp->y;
        curr->z = curr->z + temp->z;
        temp->x = (pin->vector_element->x+dimensions->x);
        temp->y = (pin->vector_element->y-dimensions->y);// + indefInt(x+a,y-b,z-c)
        temp->z = (pin->vector_element->z-dimensions->z);
        temp2 = indefintegral(temp);
        free(temp);
        temp = temp2;
        curr->x = curr->x + temp->x;
        curr->y = curr->y + temp->y;
        curr->z = curr->z + temp->z;
        temp->x = (pin->vector_element->x-dimensions->x);
        temp->y = (pin->vector_element->y-dimensions->y);// - indefInt(x-a,y-b,z-c)
        temp->z = (pin->vector_element->z-dimensions->z);
        temp2 = indefintegral(temp);
        free(temp);
        temp = temp2;
        curr->x = curr->x - temp->x;
        curr->y = curr->y - temp->y;
        curr->z = curr->z - temp->z;
        //ki->next = NULL;
        //ki->vector_element = curr;
        //ki = ki->next;
        add_vector(&ki_start,&ki,curr);
        pin = pin->next;
     }
    free(temp);
    return ki_start;
}

vect * indefintegral(vect * pin){
    vect * indef = (vect*)malloc(sizeof(vect));
    double a = 0.00;

    a = sqrt(pin->x*pin->x+ //frequently used
            pin->y*pin->y+
            pin->z*pin->z);

    if((a != 0) && (pin->y != 0) && (pin->z != 0)){
        indef->x = (pin->y - pin->x*atan(pin->y/pin->x) + pin->x*atan(pin->y*pin->z/(pin->x*a))
                   - pin->z*log(2*(pin->y+a)) - pin->y*log(2*(pin->z+a)));
        indef->y = (pin->z - pin->y*atan(pin->z/pin->y) + pin->y*atan(pin->z*pin->x/(pin->y*a))
                   - pin->x*log(2*(pin->z+a)) - pin->z*log(2*(pin->x+a)));
        indef->z = (pin->x - pin->z*atan(pin->x/pin->z) + pin->z*atan(pin->x*pin->y/(pin->z*a))
                   - pin->y*log(2*(pin->x+a)) - pin->x*log(2*(pin->y+a)));
    }
    else{
        indef->x = 0;
        indef->y = 0;
        indef->z = 0;
    }

    return indef;
}

/*                                (Elevation)
 * return a vector in form (Azimuth, Altitude, Radius)
 *                                  (x,y,z)
 */
vect * cartesianToSpherical(vect * cart){
    vect * spherical = (vect*)malloc(sizeof(vect));
    double r = 0;
    spherical->x = atan2(cart->y/cart->x,0.0000000);            //TODO: read up atan2 and atan functionality!
    r = sqrt((cart->x)*(cart->x) + (cart->y)*(cart->y) + (cart->z)*(cart->z));
    spherical->y = asin(cart->z/r);
    spherical->z = r;
    return spherical;
}
