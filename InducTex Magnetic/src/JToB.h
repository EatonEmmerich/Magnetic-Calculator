#ifndef JTOB_H_INCLUDED
#define JTOB_H_INCLUDED
position_vector * calculateBfield(vect_list* calc_position, position_vector * current_density, vect_list * dimensions);
vect * indefintegral(vect * pin);
vect_list * kicalc(vect_list * pin, vect * dimensions);


#endif // JTOB_H_INCLUDED
