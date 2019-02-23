# include "r_kernels.h"

/*	cubes_f func:
*
*	Returns a cube function with the cubes of input function, with the
*	term given (This term is not 11). Gets pointer to int, to return also
*	the number of cubes.
*/
cube_list_node *cubes_f ( cube_list_node *function, int term, int *insertions )
{

	cube_list_node *result;
	cube_list_node *temp;

	// Initialize //
	result = cube_list_initialize();
	*insertions = 0;

	// Go across the given function to check //
	for ( temp = function -> next; temp != NULL; temp = temp -> next )
	{

		// If the term excists should be 01 or 10 //
		if ( temp -> cube [term] == '0' ||  temp -> cube [ term + 1] == '0' )
		{	
			/* If both are 0 (00), the cube is invalid and the function should stop 
			*  Returning NULL
			*/
			if ( not_valid_cube ( temp -> cube ) )
			{
				cube_list_destroy ( result );
				return NULL;
			}

			cube_list_insert ( result, temp -> cube );
			*insertions = *insertions + 1;

		}
	}

	// If there are no insertions, there is no reason to allocate memory //
	if ( *insertions == 0 )
	{
		cube_list_destroy ( result );
		return NULL;
	}

	return result;
}

/*	largest_common_cube_f func:
*
*	Returns a cube function with the cubes of input function, with the
*	term given (This term is not 11). Gets pointer to int, to return also
*	the number of cubes.
*/
char *largest_common_cube_f ( cube_list_node *function )
{

	int i;
	int cube_length;

	int is_common;

	char *cube_res;

	cube_list_node *temp;

	// Check for valid input //
	if ( function == NULL )
		return NULL;
	if ( function -> next == NULL )
		return NULL;
	for ( temp = function -> next; temp != NULL; temp = temp -> next )
	{
		if ( not_valid_cube ( temp -> cube ) )
			return NULL;
	}

	// Take cube length //
	cube_length = strlen ( function -> next -> cube );

	cube_res = ( char *)malloc ( sizeof ( char ) * ( cube_length + 1 ) );
	
	// We check for each term of the first cube, if this is common in all cubes //
	for ( i = 0; i < cube_length; i = i + 2 )
	{
		// If term == 01 //
		if ( function -> next -> cube [i] == '0' )
		{
			// Check if term is 01 for all cubes //
			is_common = 1;
			for ( temp = function -> next -> next; temp != NULL; temp = temp -> next )
			{
				if ( temp -> cube [i] == '1')
				{
					is_common = 0;
				}
			}

			if ( is_common == 1 )
			{

				cube_res [i]   = '0';
				cube_res [i+1] = '1';

			}
			else
			{

				cube_res [i]   = '1';
				cube_res [i+1] = '1';

			}
		}
		
		// If term == 10 //
		else if ( function -> next -> cube [i + 1] == '0' )
		{

			// Check if term is 10 for all cubes //
			is_common = 1;
			for ( temp = function -> next -> next; temp != NULL; temp = temp -> next )
			{
				if ( temp -> cube [i + 1] == '1')
				{
					is_common = 0;
				}
			}

			if ( is_common == 1 )
			{

				cube_res [i]   = '1';
				cube_res [i+1] = '0';

			}
			else
			{

				cube_res [i]   = '1';
				cube_res [i+1] = '1';

			}
		}
		// If term == 11, put 11 //
		else
		{
			cube_res [i]   = '1';
			cube_res [i+1] = '1';
		}
	}

	cube_res [ cube_length ] = '\0';

	return cube_res;

}

char *r_kernels_fromStr ( char **in_function_str, int cubes_num )
{

	unsigned int i;

	char *final_string;

	cube_list_node * in_function;
	table_cube_functions kernels_functions_table;

	in_function = cube_list_initialize ();
	table_cube_functions_intialize( &kernels_functions_table );

	for ( i = 0; i < cubes_num; i++ )
	{
		cube_list_insert ( in_function, in_function_str[i] );
	}

	r_kernels ( 0, in_function, &kernels_functions_table );

	final_string = table_cube_functions_getChar ( kernels_functions_table );
	table_cube_functions_destroy ( &kernels_functions_table );

	return final_string;
}

/*	r_kernels func:
*
*	The implementation of r_kernels function, but we check the term to get to
*	the next one.
*/
table_cube_functions *r_kernels ( int first_term, cube_list_node *in_function, table_cube_functions *kernels_table )
{

	int term;
	int cube_length;
	int cubes_cost;

	cube_list_node *cubes_res;

	cube_list_node *division_co_kernel;
	cube_list_node **division_res;
	cube_list_node *division_res_q;

	char *co_kernel;

	// If input in NULL, return NULL //
	if ( in_function == NULL )
	{
		return NULL;
	}
	if ( in_function -> next == NULL )
	{
		return NULL;
	}

	// Get cube length //
	cube_length = strlen ( in_function -> next -> cube );

	// Check for each term //
	for ( term =  first_term; term < cube_length; term = term + 2)
	{

		/*	 Get result of CUBES function. Do not continue if
		*	 number of cubes in result is smaller than 2 
		*/
		cubes_res = cubes_f ( in_function, term, &cubes_cost);
		if ( cubes_res == NULL )
		{
			continue;
		}
		if ( cubes_cost > 1 )
		{

			// Get co Kernel //
			co_kernel = largest_common_cube_f ( cubes_res );

			// And make it as a cube function to use it in division //
			division_co_kernel = cube_list_initialize();
			cube_list_insert ( division_co_kernel, co_kernel );

			// Make division and get quontient //
			division_res = division_algebric ( in_function, division_co_kernel, 1);
			division_res_q = division_res[0];

			// Continue recursion. The result is added in kernels table in	 //
			// the called function						 //
			r_kernels ( term, division_res_q, kernels_table );

			cube_list_destroy ( division_co_kernel );
			cube_list_destroy ( division_res[0] );
			cube_list_destroy ( division_res[1] );
			free ( division_res );
			cube_list_destroy ( cubes_res );

		}
		// If cubes_f returned cube function with 1 cube. Destroy the function //
		else
		{
			cube_list_destroy ( cubes_res );
		}

	}

	// Insert the function as kernel in kernels_table //
	table_cube_functions_insert ( kernels_table, in_function );

	return kernels_table;



}
