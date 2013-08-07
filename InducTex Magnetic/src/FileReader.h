#ifndef FILEREADER_H_INCLUDED
#define FILEREADER_H_INCLUDED

void get_data(position_vector ** data_list_start,vect_list ** dimensions_start,char filename[]);
vect_list * get_vect_list(char * filename);
int save_position_vector(position_vector * input, char * filename);

#endif // FILEREADER_H_INCLUDED
