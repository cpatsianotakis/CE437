#ifndef FDS_RKERNELS_L
#define FDS_RKERNELS_L

#include "../main.h"

cube_list_node *cubes_f ( cube_list_node *function, int term, int *insertions );
char *largest_common_cube_f ( cube_list_node *function );
char *r_kernels_fromStr ( char **in_function_str, int cubes_num );
table_cube_functions *r_kernels ( int first_term, cube_list_node *in_function, table_cube_functions *kernels_table );


#endif