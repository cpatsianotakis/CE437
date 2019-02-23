#ifndef FDS_DIVAL_L
#define FDS_DIVAL_L

#include "../main.h"

cube_list_node *multiplication ( cube_list_node *first, cube_list_node *second );
char *includes_in ( char *included, char *includer);
cube_list_node **division_algebric ( cube_list_node *divisor, cube_list_node *dividend, int num_dividend_cubes);
void division_algebric_fromStr ( char **divisor_str, char **dividend_str, int num_divisor_cubes, int num_dividend_cubes);

#endif
