#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
vect_list * bounded_position_interval(position_vector* bounds, double xi, double yi, double zi);
vect_list * create_calc_position( double xl, double yl, double zl, double xh, double yh, double zh, double xi, double yi, double zi);
vect_list * bounded_position(position_vector* bounds);
int get_high_low(position_vector* bounds, double* highx, double* lowx, double* highy, double* lowy, double* highz, double* lowz);
void  print_help();
int get_length(position_vector * input_field);
#endif // MAIN_H_INCLUDED
