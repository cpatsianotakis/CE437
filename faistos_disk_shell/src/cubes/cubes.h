#ifndef FDS_CUBE_L
#define FDS_CUBE_L

struct cube_list_node_struct
{
	struct cube_list_node_struct *next;
	struct cube_list_node_struct *prev;
	char *cube;
};
typedef struct cube_list_node_struct cube_list_node;


struct table_cube_functions_struct
{
	int size;
	cube_list_node **function;
};
typedef struct table_cube_functions_struct table_cube_functions;

#include "../main.h"
	
cube_list_node *head;
cube_list_node *tail;

int cube_list_contents( cube_list_node *head);
cube_list_node *cube_list_initialize();
void cube_list_print( cube_list_node *head);
char *cube_list_getChar ( cube_list_node *head);
void cube_list_destroy(cube_list_node *head);
int cube_list_insert ( cube_list_node *head, char *new_cube);
int cube_list_remove ( cube_list_node *head, char *cube_to_remove);
cube_list_node *cube_list_search_node ( cube_list_node *head, char *cube);
cube_list_node *cube_functions_union ( cube_list_node *head_1, cube_list_node *head_2 );
cube_list_node *cube_functions_intersection ( cube_list_node *head_1, cube_list_node *head_2 );

void table_cube_functions_intialize( table_cube_functions *table );
void table_cube_functions_insert ( table_cube_functions *table, cube_list_node *head );
char *table_cube_functions_getChar ( table_cube_functions table );
void table_cube_functions_union ( table_cube_functions *table1, table_cube_functions *table2 );
void table_cube_functions_destroy ( table_cube_functions *table );

// Internal Functions
int not_valid_cube(char *cube);
int double_zeros ( char *cube);
char *make_intersect ( char *argStr1, char *argStr2);
char *make_supercube ( char *argStr1, char *argStr2);
int check_coverage ( char *argStr1, char *argStr2);
cube_list_node *make_sharp_list( char *argStr1, char *argStr2);
cube_list_node *sharp_cube_function ( char *cube, char **function, int num_function_cubes);
cube_list_node *get_OFF_f ( cube_list_node *function );

#endif
