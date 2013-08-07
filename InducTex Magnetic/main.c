/*
 *  This program uses the equations and algorithms found in
 *  "Mean square flux noise in SQUIDs and qubits: numerical
 *  calculations" by S M Anton, I A B Sognnaes,
 *  J S Birenbaum, S R O’Kelley,C J Fourie and John Clarke.
 *  to determine the magnetic field intensity at given points
 *  induced by a current density matix recieved by InducEx.
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
#include "main.h"

vect_list * create_calc_position(double x, double y, double z, double xi, double yi, double zi);

int main(int argc, char *argv[])
{
    position_vector * j_field = NULL;  //imported j field
    position_vector * b_field;  //calculated b_field
    vect_list * calc_position;  //position for b_field to be calculated
    vect_list * dimensions = NULL;     //description of j field segments
    char * filename;

    if (!(argc == 2 || argc == 4 || argc == 9)) // argc should be 2 or 4 or 9 for correct execution
    {
        // We print argv[0] assuming it is the program name
        printf("Usage: %s \"current density filename\" ([-p 'position filename'] OR ", argv[0] );
        printf("[-m 'x size' 'y size' 'z size' 'x intervals' 'y intervals' 'z interval'])");
        printf( "\n\texample: %s ./currdens.txt", argv[0]);
        return -1;
    }
    else
    {
        // We assume argv[1] is a filename to open
        filename = argv[1];
        if(argc == 4 || argc == 9){
                printf("\ninside 4&9\n");
            switch(argv[2][1]){
                case 'p' : printf("\ninside 4&9\n");
                    calc_position = get_vect_list(argv[3]);
                            break;
                case 'm' : calc_position = create_calc_position((double) (*argv[3]),(double) (*argv[4]),(double) (*argv[5]),(double) (*argv[6]),(double) (*argv[7]),(double) (*argv[8]));
                            break;
                default :  printf("\nIncorrect parameters, please use as follow:");
                printf("\n%s \"current density filename\" [-p 'position filename']/", argv[0]);
                printf("\n[-m 'x size' 'y size' 'z size' 'x intervals' 'y intervals' 'z interval']");
                    return -2;
            }
            printf("\npast switch\n");
        }
        else
        {
            printf("Using default matrix of (10 10 10) (0.1 0.1 0.1)");
            calc_position = create_calc_position(10, 10, 10, 0.1, 0.1, 0.1);
        }
    }
    printf("\nUsing position with data in index 1: %lf %lf %lf\n", calc_position->vector_element->x, calc_position->vector_element->y, calc_position->vector_element->z);
    get_data(&j_field,&dimensions,filename);
    b_field = calculateBfield(calc_position, j_field, dimensions);
    printf("\ncompleted calculating B field.\nSaving data to disk.");
    filename = strcat(filename,"BF");
    save_position_vector(b_field,filename);

    return 0;
}

vect_list * create_calc_position(double x, double y, double z, double xi, double yi, double zi){
    vect_list * position = NULL;
    vect_list * position_start = NULL;
    double xtmp, ytmp, ztmp;

    xtmp = x;
    while(xtmp/xi >= 1){
        ytmp = y;
        while(ytmp/yi >= 1){
            ztmp = z;
            while(ztmp/zi >= 1){
                add_vector_val(&position_start, &position, xtmp, ytmp, ztmp);
                ztmp = ztmp - zi;
            }
            ytmp = ytmp - yi;
        }
        xtmp = xtmp - xi;
    }
    return position_start;
}
