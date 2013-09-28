#ifndef COORDINATESCALCULATOR_H_INCLUDED
#define COORDINATESCALCULATOR_INCLUDED
vect_list * bounded_position_interval(position_vector* bounds, double xi, double yi, double zi);
vect_list * create_calc_position( double xl, double yl, double zl, double xh, double yh, double zh, double xi, double yi, double zi);
vect_list * bounded_position(position_vector* bounds);
int get_high_low(position_vector* bounds, double* highx, double* lowx, double* highy, double* lowy, double* highz, double* lowz);
int get_high_low_list(vect_list* bounds, double* highx, double* lowx, double* highy, double* lowy, double* highz, double* lowz);
position_vector * concatanate_b_field(position_vector * b_field[],int length);

#endif // COORDINATESCALCULATOR_H_INCLUDED
