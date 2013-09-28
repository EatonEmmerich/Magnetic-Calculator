/*//Caps parameters
//
 *  This program uses the equations and algorithms found in
 *  "Mean square flux noise in SQUIDs and qubits: numerical
 *  calculations" by S M Anton, I A B Sognnaes,
 *  J S Birenbaum, S R O’Kelley,C J Fourie and John Clarke.
 *  to determine the magnetic field intensity at given points
 *  induced by a current density matix recieved by InducEx.
 *  The program was first made in Matlab by *** and then
 *  re-written into C by Eaton Meyer Emmerich.
 *
 *  This program is used through a command terminal with the
 *  following parameters:
 *  % "InducTex Magnetic.exe" 'current filename' [-p 'position filename']/
 *  [-m 'x size' 'y size' 'z size' 'x intervals' 'y intervals' 'z interval']
 *
 */


#include "src/DataTypes.h"
#include "src/FileReader.h"
#include "src/JToB.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "main.h"
#define MAX_THREADS 16

int main(int argc, char *argv[])
{
    position_vector * j_field [MAX_THREADS];  //imported j field
    position_vector * j_field_total = NULL;
    position_vector * j_field_last = NULL;
    position_vector * b_field[MAX_THREADS];  //calculated b_field
    position_vector * b_field_total;
    vect_list * calc_position = NULL;  //position for b_field to be calculated
    vect_list * calc_position_dup[MAX_THREADS];
    vect_list * calc_position_dup_last = NULL;
    vect_list * calc_position_temp = calc_position;
    vect_list * dimensions_total = NULL;     //description of j field segments
    vect_list * dimensions[MAX_THREADS];
    vect_list * dimensions_last = NULL;
    char * filename;
    char flag = 0;
    char isvalidfile = 0;
    double coords[9];
    int coordcounter = 0;
    char * inputvalid;
    int num_threads = 2;
    int threadcounter = 0;
    int sizecounter = 0;
    int input_length = 0;

    //initialize j_field and b_fields to NULL
    for (coordcounter = 0; coordcounter < MAX_THREADS; coordcounter++){
        j_field[coordcounter] = NULL;
        b_field[coordcounter] = NULL;
        dimensions[coordcounter] = NULL;
    }


    if (!(argc == 2 || argc == 3 || argc == 4 || argc == 12 || argc == 6)) // argc should be 2 or 4 or 9 for correct execution
    {
        // We print argv[0] assuming it is the program name
        printf("Incorrect Syntax Error.");
        printf("Usable Syntax: %s (current density filename) ([-p] [-m] [-h]) (arg1 arg2 arg3 ...)",argv[0]);
        printf( "\n\texample: %s ./currdens.txt\nFor more information use %s -h", argv[0],argv[0]);
        return -1;
    }
    else
    {
        // We assume argv[1] is a filename to open
        filename = argv[1];
        if(argc > 2){
            if(argv[2][0] == ('-') && argv[2][2] == NULL){
                //printf("\ninside switch if\n");
                switch(argv[2][1]){
                    case 'p' :
                        printf("\ninside p case\n");
                        if(argc == 4){
                            flag = 0;
                        }else{
                            printf("Usable Syntax: %s (current density filename) ([-p] [-m] [-b] [-help]) (arg1 arg2 arg3 ...)",argv[0]);
                            printf( "\n\texample: %s ./currdens.txt\nFor more information use %s -help", argv[0],argv[0]);
                            return -2;
                        }
                                break;
                    case 'm' :
                        printf("\ninside m case  %i\n", argc);
                        if(argc == 12){
                            flag = 1;
                        }else if(argc == 6){
                            flag = 2;
                        }else if(argc == 3){
                            printf("flag = 3");
                            flag = 3;
                        }else{
                            printf("\nIncorrect parameters, please use one of the following:");
                            print_help();
                            return -2;
                        }
                                break;
                    default :  printf("\nIncorrect parameters, please use one of the following:");
                    print_help();
                        return -2;
                }
                printf("\npast switch\n");
            }
            else
            {
                printf("\nIncorrect parameters, please use one of the following:");
                print_help();
            }
        }
        else{
            //printf("else");
            if(argv[1][0] == ('-') && argv[1][2] == NULL){
                switch(argv[1][1]){
                    case 'h' :
                        printf("\nWelcome to Magnetic field solver help. The program is used as follows:");
                        printf("\nUsable Syntax: %s (current density filename) ([-p] [-m] [-h]) (arg1 arg2 arg3 ...)",argv[0]);
                        printf( "\n\texample: %s ./currdens.txt\nFor more information use %s -h", argv[0],argv[0]);
                        print_help();
                        return 0;
                    default :  printf("\nIncorrect parameters, please use one of the following:");
                    print_help();
                        return -2;
                }
            }
            else{
                printf("\nIncorrect parameters, please use one of the following:");
                print_help();
                return -2;
            }
        }
    }

    if(get_data(&j_field_total,&dimensions_total,filename) == 1) {
            isvalidfile = 1;
            //printf("isvalid");
    }
    if(flag == 0 && isvalidfile){
        printf("swww");
        if(get_vect_list(argv[3],&calc_position) == 1){
        }
        else{
            isvalidfile = 0;
        }
    }

    else if(flag == 1){
        for(coordcounter = 0; coordcounter < 9; coordcounter ++){
            coords[coordcounter] = strtod(argv[3+coordcounter], &inputvalid);
            if(inputvalid == argv[3]){
                printf("\nCould not interpret number %s", inputvalid);
                return -3;
            }
        }
        if(coords[6] == 0 || coords[7] == 0 || coords[8] == 0){
            printf("\nImpossible to have a interval size of zero.");
            return -4;
        }
        calc_position = create_calc_position(coords[0], coords[1], coords[2], coords[3], coords[4], coords[5], coords[6], coords[7], coords[8]);
    }
    else if(flag == 2 && isvalidfile){
        for(coordcounter = 0; coordcounter < 3; coordcounter ++){
            coords[coordcounter] = strtod(argv[3+coordcounter], &inputvalid);
            if(inputvalid == argv[3]){
                printf("\nCould not interpret number %s", inputvalid);
                return -3;
            }
        }
        if(coords[0] <= 0 || coords[1] <= 0 || coords[2] <= 0){
            printf("\nImpossible to have a interval size of zero or less.");
            return -4;
        }
        calc_position = bounded_position_interval(j_field_total,coords[0], coords[1], coords[2]);
    }
    else if(flag == 3 && isvalidfile)calc_position = bounded_position(j_field_total);

    if(isvalidfile){
        printf("\nUsing position with data in index 1: %lf %lf %lf\n", calc_position->vector_element->x, calc_position->vector_element->y, calc_position->vector_element->z);
        printf("\nUsing dim with data in index 1: %lf %lf %lf\n", dimensions_total->vector_element->x, dimensions_total->vector_element->y, dimensions_total->vector_element->z);

        /***********************************************
         *        Added threaded functionality:        *
         ***********************************************/
        input_length = get_length(j_field_total);
        for(threadcounter = 0; threadcounter < num_threads; threadcounter++){
            dimensions_last = dimensions[threadcounter];
            j_field_last = j_field[threadcounter];
            printf("\n\nThread %d initialized\n\n", threadcounter);
            for(sizecounter = 0; sizecounter < (input_length / num_threads); sizecounter++){
                if((j_field_total != NULL) && (dimensions_total != NULL)){
                    add_position_vector_pointer(&(j_field[threadcounter]),&j_field_last,j_field_total);
                    //j_field[threadcounter] = j_field_total;
                    add_vector(&(dimensions[threadcounter]),&dimensions_last,dimensions_total->vector_element);
                    //dimensions[threadcounter] = dimensions_total;
                    j_field_total = j_field_total->next;
                    dimensions_total = dimensions_total->next;
                }

            }
            /*printf("calc_position %d", threadcounter);
            calc_position_temp = calc_position;
            while(calc_position_temp != NULL){
                add_vector(&(calc_position_dup[threadcounter]),&calc_position_dup_last,calc_position_temp->vector_element);
                printf("calc_position");
                calc_position_temp = calc_position_temp->next;
            }*/
        }


       // omp_set_num_threads(num_threads);
       // printf("%d",num_threads);
        #pragma omp parallel for
        {
        for(threadcounter=0;threadcounter<num_threads;threadcounter++) {//A,B,C such that total iterations known at start of loop
            //<your code here>
            //<force ordered execution of part of the code. A=C willbe guaranteed to execute
            //before A=C+1>
            printf("\n\nThread %d starting\n\n", threadcounter);
                b_field[threadcounter] = calculateBfield(calc_position, j_field[threadcounter], dimensions[threadcounter]);
            printf("\n\nThread %d started\n\n", threadcounter);
        }
        }


            //b_field = calculateBfield(calc_position, j_field[], dimensions[]);
            printf("\ncompleted calculating B field.\nSaving data to disk.");
            filename = strcat(filename,"BF");
            /*
            Put b_field into b_field total.
            */
            save_position_vector(b_field_total,filename);
        }

    /*
    else{
        printf("Usable Syntax: %s (current density filename) ([-p] [-m] [-b] [-help]) (arg1 arg2 arg3 ...)",argv[0]);
        printf( "\n\texample: %s ./currdens.txt\nFor more information use %s -help", argv[0],argv[0]);
        return -1;
    }*/
    printf("press any key to continue.");
    scanf("%c",inputvalid);
    return 0;
}

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
        //printf("YH");
        while(ytmp > yl){
            ztmp = zh;
            while(ztmp > zl){
                add_vector_val(&position_start, &position, xtmp, ytmp, ztmp);
                if(ztmp - zi <= zl){
                    ztmp = zl;
                }
                else{
                    ztmp = ztmp - zi;
                }
            }
            if(ytmp - yi < yl){
                ytmp = yl;
            }else{
                ytmp = ytmp - yi;
            }
        }
        if(xtmp - xi < xl){
            xtmp = xl;
        }else{
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

void print_help(){
    printf("\n------------------------------------------------------\n\n");
    printf("\t -p [filename]\t\tUse file with coordinates to\n\t\t\t\tcalculate the magnetic field for each\n\t\t\t\tcoordinate therein.");
    printf("\n\n");
    printf("\t -m [arguments]\t\tCreate a square matrix of coordinates\n\t\t\t\tfor use, to calculate the magnetic\n\t\t\t\tfield for each coordinate therein.");
    printf("\n\narguments include:\n**[(low x) (low y) (low z) (high x) (high y) (high z) ...\n   (x interval) (y interval) (z interval)]");
    printf("\n\n**[(x interval) (y interval) (z interval)]\tWill automatically bound to\n\t\t\t\t\t\tcurrent density input values.");
    printf("\n\n**[ ]\tWill automatically bound to current density input values with 10\n\tintervals in each dimension.");

}

int get_length(position_vector * input_field){
    int length = 0;
    while (input_field != NULL){
        length ++;
        input_field = input_field->next;
    }
    return length;
}
