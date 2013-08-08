#include <stdio.h>
#include <stdlib.h>
#include "DataTypes.h"
/*
 * Insert data into datalist.
 */
int get_data(position_vector ** data_list_start,vect_list ** dimensions_start,char filename[]){
    position_vector * data_list_current = *data_list_start;
    vect_list * dimensions_curr = *dimensions_start;
    FILE * inputfile;
    int i = 0;
    double px, py, pz, jx, jy, jz, dx, dy, dz;
    printf("Opening file: %s", filename);
    inputfile = fopen(filename,"r");
    if(inputfile == NULL){
        printf(".....       Could not open file,\nthe file is either missing or not available at this moment");
        return 0;
    }
    else{
        printf(".....       File opened.");
        printf("\nConverting data into respective structures.\n");
        while(!feof(inputfile)){
            i++;
            fscanf(inputfile, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &px, &py, &pz, &jx, &jy, &jz, &dx, &dy, &dz);

            add_position_vector(data_list_start, &data_list_current, px, py, pz, jx, jy, jz);
            add_vector_val(dimensions_start, &dimensions_curr, dx, dy, dz);
        }
        printf("\nCompleted conversion at Line %i of file.", i);
    }
    fclose(inputfile);
    return 1;
}

int get_vect_list(char * filename, vect_list ** vectlist_start){
    vect_list * vectlist = NULL;
    *vectlist_start = NULL;
    double px, py, pz;
    FILE * inputfile;
    printf("\nOpening file \'%s\' for area to calculate.", filename);
    inputfile = fopen(filename,"r");
    if(inputfile == NULL){
        printf(".....       Could not open file,\nthe file is either missing or not available at this moment");
        return 0;
    }
    else{
        printf(".....       File opened.");
        printf("\nConverting data into respective structures.\n");
        while(!feof(inputfile)){
            fscanf(inputfile, "%lf %lf %lf", &px, &py, &pz);
            add_vector_val(vectlist_start, &vectlist, px, py, pz);
        }
        printf("\nCompleted conversion of file.");
    }
    fclose(inputfile);
    return 1;
}

int save_position_vector(position_vector * input, char * filename){
        FILE * outputfile;
        outputfile = fopen(filename,"w");
        if(outputfile == NULL){
            printf(".....       Could not open file,\nthe file is not available at this moment.");
            return 0;
        }
        else{
            printf(".....       Saving to %s.", filename);
        while(input != NULL){
            fprintf(outputfile, "%e,%e,%e,%e,%e,%e;\n", input->position->x, input->position->y, input->position->z, input->current_density->x, input->current_density->y,input->current_density->z);
            input = input->next;
        }
        printf("\nCompleted saving to file.");
       }
       fclose(outputfile);
       return 1;
    }
