#ifndef FDS_ESPRESSO_L
#define FDS_ESPRESSO_L

#include "../main.h"

struct row_ucp_struct
{
	int row_length;
	int *row_ptr;
};
typedef struct row_ucp_struct row_ucp;

row_ucp *initialize_ucp_table ( int rows, int cols );
row_ucp *make_ucp_table ( int **table, int rows, int cols );
void destroy_ucp_table ( row_ucp *table, int rows );
void remove_row ( row_ucp *table, int row, int *rows_num, int *cols_num );
int *ucp_lines ( row_ucp *row_ucp_table, int **int_table, int rows, int cols, int MODE);


cube_list_node *expand ( cube_list_node *f_ON, cube_list_node *f_DC );
cube_list_node *reduce ( cube_list_node *f_ON, cube_list_node *f_DC );
cube_list_node *irredundant ( cube_list_node *f_ON, cube_list_node *f_DC );
int calculate_cost ( cube_list_node *f );
cube_list_node *espresso ( cube_list_node *f_ON, cube_list_node *f_DC);



#endif