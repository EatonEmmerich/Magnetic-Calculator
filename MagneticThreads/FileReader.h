#ifndef FILEREADER_H_INCLUDED
#define FILEREADER_H_INCLUDED

int get_data(position_vector ** data_list_start,vect_list ** dimensions_start,char filename[]);
int get_vect_list(char * filename, vect_list ** vectlist_start);
int save_position_vector(position_vector * input, char * filename);

#endif // FILEREADER_H_INCLUDED
