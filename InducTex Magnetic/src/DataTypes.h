#ifndef DATAHANDLING_H_INCLUDED
#define DATAHANDLING_H_INCLUDED

struct vect_structure{
  double x;
  double y;
  double z;
};
typedef struct vect_structure vect;

struct vect_list_structure{
  vect* vector_element;
  struct vect_list_structure* next;
};
typedef struct vect_list_structure vect_list;
/*
 * The data structure to use.
 */
typedef struct data_structure {
  vect* position;
  vect* current_density;
  struct data_structure* next;
}position_vector;

void TestDataStruct();
void add_position_vector(position_vector** data_list_first, position_vector ** data_list_last, double px, double py, double pz, double jx, double jy, double jz);
void add_vector(vect_list ** data_list_first, vect_list ** data_list_last, vect * pos);
void add_vector_val(vect_list ** data_list_first, vect_list ** data_list_last, double x, double y, double z);
int data_list_size(position_vector * data_list_first);
int compare_vectors(vect * vect_a, vect * vect_b);
void free_vect_list(vect_list ** tempvect);
void halfofvect(vect_list ** input);
void position_vector_divten(position_vector ** input);
#endif // DATAHANDLING_H_INCLUDED
