#include<stdlib.h>
#include<stdio.h>
#include"DataTypes.h"

/*
 * Test these data structures:
 */
void TestDataStruct(){
    position_vector * first = NULL;
    position_vector * current = NULL;
    int i = 0;

    //create data
    add_position_vector(&first, &current, (double) i, (double) i, (double) i, 3.17, 3.17, 3.17);

    for(i = 1;i <= 10;i ++) {

      add_position_vector(&first, &current, (double) i, (double) i, (double) i, 3.17, 3.17, 3.17);
   }
   current->next = 0;

   //print data
    current = first;
    printf("Position in (x,y,z)\n");

   while(current) {
      printf("(%f, ", current->position->x);
      printf("%f, ", current->position->y);
      printf("%f)\n", current->position->z);
      current = current->next;
   }
   printf("Size of data list: %i\n", data_list_size(first));
}

void add_position_vector(position_vector** data_list_first, position_vector ** data_list_last, double px, double py, double pz, double jx, double jy, double jz){
    vect * position = NULL;
    vect * curdens = NULL;
    position_vector * newdata = NULL;
    position = (vect* )malloc(sizeof(vect));
    curdens = (vect* )malloc(sizeof(vect));
    newdata = (position_vector *)malloc(sizeof(position_vector));

    if(newdata != NULL || position != NULL || newdata != NULL){    //enough memory available.
        position->x = px;
        position->y = py;
        position->z = pz;
        curdens->x = jx;
        curdens->y = jy;
        curdens->z = jz;
        newdata->next = NULL;
        newdata->position = position;
        newdata->current_density = curdens;
        if(*data_list_first != NULL){//not first element
            (*data_list_last)->next = newdata;
            *data_list_last = newdata;
        }else{
            *data_list_first = newdata;
            *data_list_last = *data_list_first;
        }
    }
    else{
        printf("\nout of memory error.\n");
    }
}

void free_all(position_vector* data_list_first){
    position_vector* temp = data_list_first->next;

    while(temp != NULL){
        free(data_list_first->position);
        free(data_list_first->current_density);
        free(data_list_first);
        data_list_first = temp;
        temp = data_list_first->next;
    }

    free(data_list_first->position);
    free(data_list_first->current_density);
    free(data_list_first);
}

 /*
  * free the memory the temporary vector holds.
  */
void free_vect_list(vect_list ** tempvect){
    vect_list *temp, *temp2;
    temp = *tempvect;
    while (temp != NULL) {
        free(temp->vector_element);
        temp2 = temp;
        temp = temp->next;
        free(temp2);
    }
    //printf("..");
}

int data_list_size(position_vector * data_list_first){
    int length = 0;

    if(data_list_first != NULL){
        while (data_list_first != NULL){
            length++;
            data_list_first = data_list_first->next;
        }
    }
    return length;
}

int data_list_unique_size(position_vector * data_list_first){
    vect_list * uniques_first = NULL;
    vect_list * uniques_curr = NULL;
    vect_list * uniques_last = NULL;
    int flag;
    int usize = 0;

    while(data_list_first != NULL){
        uniques_curr = uniques_first;
        flag = 0;
        while(uniques_curr != NULL){
            if(compare_vectors(uniques_curr->vector_element, data_list_first->position)){
                flag++;
                break;
            }
            uniques_curr = uniques_curr->next;
        }
        if(!flag){
            add_vector(&uniques_first,&uniques_last,data_list_first->position);
            uniques_last = uniques_curr;
            usize ++;
        }
    }
    return usize;
}

void add_vector(vect_list ** data_list_first, vect_list ** data_list_last, vect * pos){
    vect_list * newdata = NULL;
    newdata = (vect_list *)malloc(sizeof(vect_list));
    if(newdata != NULL){    //enough memory available.
        newdata->next = NULL;
        newdata->vector_element = pos;
        if(*data_list_first != NULL){//not first element
            (*data_list_last)->next = newdata;
            *data_list_last = (*data_list_last)->next;
        }else{
            *data_list_first = newdata;
            *data_list_last = *data_list_first;
        }
    }
    else{
        printf("\nnot enough memory error.\n");
    }
}

void add_vector_val(vect_list ** data_list_first, vect_list ** data_list_last, double x, double y, double z){
    vect * position = NULL;
    vect_list * newdata = NULL;
    newdata = (vect_list *)malloc(sizeof(vect_list));
    position = (vect* )malloc(sizeof(vect));

    if(newdata != NULL || position != NULL){    //enough memory available.
        position->x = x;
        position->y = y;
        position->z = z;
        newdata->next = NULL;
        newdata->vector_element = position;
        if(*data_list_first != NULL){//not first element
            (*data_list_last)->next = newdata;
            *data_list_last = newdata;
        }else{
            *data_list_first = newdata;
            *data_list_last = *data_list_first;
        }
    }else{
        printf("\nnot enough memory error.\n");
    }
}

int compare_vectors(vect * vect_a, vect * vect_b){
    int i = 0;
    if(vect_a->x == vect_b->x)
        if(vect_a->y == vect_b->y)
            if(vect_a->z == vect_b->z)
                    i = 1;

    return i;
}

/*
 * Half the capacity of all vectors in a vect_list
 *
 */
void halfofvect(vect_list ** input){
     vect_list * inputstart = *input;

    while(*input != NULL){
        (*input)->vector_element->x = (*input)->vector_element->x/2;
        (*input)->vector_element->y = (*input)->vector_element->y/2;
        (*input)->vector_element->z = (*input)->vector_element->z/2;
        (*input) = (*input)->next;
    }
    *input = inputstart;
 }

/*
 * division by 10 the capacity of all density's in a posision_vector
 *
 */
void position_vector_divten(position_vector ** input){
     position_vector * inputstart = *input;

    while(*input != NULL){
        (*input)->current_density->x = (*input)->current_density->x*0.1;
        (*input)->current_density->y = (*input)->current_density->y*0.1;
        (*input)->current_density->z = (*input)->current_density->z*0.1;
        (*input) = (*input)->next;
    }
    *input = inputstart;
 }
