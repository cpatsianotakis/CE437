#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl8.6/tcl.h>

#include "cubes.h"

/*	not_valid_cube func:
*
*	Checks for each cube, if:
*		-> Its contents are only '0' or '1'
*		-> Its length is even
*		-> It doesn't have double zeros
*
*	Returns if is valid 0, else 1
*/
int not_valid_cube(char *cube)
{
	int cube_length = strlen(cube);
	
	// Check if even //
	if ( ( cube_length % 2) != 0 )
		return 1;
	
	// Check if has only 0, 1 //
	for ( int i = 0; i < cube_length; i++)
	{
		if ( cube[i] != '0' && cube[i] != '1')
			return 1;
	}
	
	// Check for double zeros //
	if( double_zeros( cube) != 0)
		return 1;

	return 0;
}

/*	double_zeros func:
*
*	Calculates the double zeros in cube
*
*	Returns the calculated number
*/
int double_zeros( char *cube)
{
	unsigned int i;

	int double_zeros = 0;
	int cube_variants = strlen( cube) / 2;

	for( i = 0 ; i < cube_variants; i++){
		if( cube[i*2] == '0' && cube[i*2 + 1] == '0')
			double_zeros++;
	}

	return double_zeros;
}

/*	make_intersect func:
*
*	Returns new cube(string) as intersect
*	of two input cubes(strings)
*/
char *make_intersect (char *argStr1, char *argStr2)
{

	unsigned int i; 

	int cube_length = strlen( argStr1);
	char new_cube[cube_length + 1];

	for(i = 0; i < cube_length; i++){
		
		new_cube[i] = AND ( argStr1[i], argStr2[i] );

	}
	new_cube[cube_length] = '\0';

	return strdup(new_cube);
}

/*	make_supercube func:
*
*	Returns new cube(string) as supercube
*	of two input cubes(strings)
*/
char *make_supercube (char *argStr1, char *argStr2)
{

	unsigned int i;

	int cube_length = strlen( argStr1);
	char new_cube[cube_length + 1];

	for(i = 0; i < cube_length; i++){

		new_cube[i] = OR ( argStr1[i], argStr2[i] );
	}

	new_cube[cube_length] = '\0';

	return strdup(new_cube);
}

/*	check_coverage func:
*
*	Calculates how much first cube covers the
*	second
*
*	Returns the coverage if it covers, else 0 
*/
int check_coverage ( char *argStr1, char *argStr2)
{

	unsigned int i;
	int cube_length;
	int coverage = 0;

	cube_length = strlen(argStr1);

	for( i = 0; i < cube_length; i++)
	{
		// No cover in this case //
		if(argStr1[i] == '0' && argStr2[i] == '1')
			return 0;
		// 1 bit of first and 0 of second... Coverage //
		if(argStr1[i] == '1' && argStr2[i] == '0')
			coverage++ ;
	}

	return coverage;
}

/*	make_sharp_list func:
*
*	Makes new list, which is the result of sharp
*	function between 2 cubes
*
*	Sharp function (a1a2...an # b1b2...bn) (where etc a1 is var):
*	 _______________________________________________
*	|a1*(b1')	a2		...	an	|
*	|a1		a2*(b2')	...	an	|
*	|...		...		...	...	|
*	|a1		...		...	an*(bn')|
*	|_______________________________________________|	
*
*	Returns if success the new list, else NULL
*/
cube_list_node *make_sharp_list( char *argStr1, char *argStr2)
{
	unsigned int i, j;

	cube_list_node *sharp_list;

	int cube_length = strlen(argStr1);
	char cube[ cube_length + 1];
	
	// Create the list //
	sharp_list = cube_list_initialize();
	if (sharp_list == NULL)
		return NULL;
	
	// Go across the lines //
	for( i = 0; i < cube_length ; i += 2)
	{
		// Go across the line untill diagonal //
		for( j = 0; j < i; j++)
		{
			cube[j] = argStr1[j];
		}
		
		// Calculate diagonal //
		cube[i] = AND( argStr1[i], NOT(argStr2[i]) );
		cube[i+1] = AND( argStr1[i+1], NOT(argStr2[i+1]) );
		
		// Go across the line after diagonal //
		for( j = i + 2; j < cube_length; j++)
		{	
			cube[j] = argStr1[j];
		}

		cube[ cube_length ] = '\0';
		
		// Check if cube is valid and if doesn't excist already in list and insert it //
		if( !double_zeros(cube) && ( cube_list_search_node (sharp_list, cube) == NULL ) )
		{	
			cube_list_insert(sharp_list, cube);
		}
		
	}

	return sharp_list;
}

/*	sharp_cube_funtion func:
*
*	Executes sharp function between a cube and a function:
*	a#F = (a#f1)*(a#f2)*...*(a#fn)
*
*	Returns result of function in succes, else NULL
*/
cube_list_node *sharp_cube_function ( char *cube, char **function, int num_function_cubes)
{

	unsigned int i;
	cube_list_node *result, *prev_result;
	cube_list_node **sharp_2_result;
	cube_list_node *temp_node1, *temp_node2;

	char *temp_cube;
	
	// If function has 1 cube, the func is practically sharp_2 //
	if(num_function_cubes == 1)
	{
		result = make_sharp_list ( cube, function[0]);
		return result;
	}
	
	// Allocate memory for head lists for # results //
	sharp_2_result = (cube_list_node* *)malloc( sizeof(cube_list_node**) * num_function_cubes);

	// # for each parenthesis //
	for ( i = 0; i < num_function_cubes; i++)
	{
		sharp_2_result[i] = make_sharp_list ( cube, function[i]);
		if ( sharp_2_result[i] == NULL )
			return NULL;
	}

	// Distributive property //
	prev_result = sharp_2_result[0];
	for( i = 1; i < num_function_cubes; i++)
	{	
		// Remake result list //
		result = cube_list_initialize();

		// Go across the products of first parenthesis //
		for( temp_node1 = prev_result; temp_node1 != NULL; temp_node1 = temp_node1->next)
		{
			// Skip sentinel //
			if( strcmp ( temp_node1->cube, "SENTINEL") == 0 )
				continue;
			
			// Go across the products of second parenthesis //
			for( temp_node2 = sharp_2_result[i]; temp_node2 != NULL; temp_node2 = temp_node2->next)
			{
				// Skip sentinel //
				if( strcmp ( temp_node2->cube, "SENTINEL") == 0 )
				continue;
					
				// Intersect is the * function of distributive property //
				temp_cube = make_intersect(temp_node1->cube, temp_node2->cube);

				// If result is valid and doesn't excist, insert it //
				if ( !double_zeros ( temp_cube) && ( cube_list_search_node ( result, temp_cube) == NULL ) )
					cube_list_insert(result,  temp_cube);

				free(temp_cube);
			}
		}
		
		// Destroy what is not needed more //
		cube_list_destroy ( sharp_2_result[i]);
		cube_list_destroy ( prev_result);
		prev_result = result;

	}

	free ( sharp_2_result );

	return result;
}


cube_list_node *get_OFF_f ( cube_list_node *function )
{

	int i;
	int cube_length, function_length;
	char *universal_cube;
	char **function_in_str;

	cube_list_node *temp;
	cube_list_node *OFF_function;

	cube_length = strlen ( function -> next -> cube );

	universal_cube = ( char *) malloc ( sizeof ( char ) * (cube_length + 1));

	for ( i = 0; i < cube_length; i++ )
		universal_cube [i] = '1';
	universal_cube[i] = '\0';

	function_length = cube_list_contents ( function );

	function_in_str = ( char* *) malloc ( sizeof ( char* ) * function_length);

	for ( temp = function -> next, i = 0; i < function_length && temp != NULL; temp = temp -> next, i++ )
	{
		function_in_str[i] = strdup ( temp -> cube );
	}

	OFF_function = sharp_cube_function ( universal_cube, function_in_str, function_length);

	free ( universal_cube );

	for ( i = 0; i < function_length; i++ )
	{
		free ( function_in_str[i] );
	}
	free ( function_in_str );

	return OFF_function;

}


