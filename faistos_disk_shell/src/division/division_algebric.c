# include "division_algebric.h"

/*	multiplication func:
*
*	Returns the result of multiplication of 2 cube functions
*	as a new cube function.
*/
cube_list_node *multiplication ( cube_list_node *first, cube_list_node *second )
{

	cube_list_node *temp_A, *temp_B;
	cube_list_node *result;
	char *temp_cube;

	result = cube_list_initialize();

	// Go accross the first function //
	for ( temp_A = first -> next; temp_A != NULL; temp_A = temp_A -> next )
	{

		// Go accross the second function //
		for ( temp_B = second -> next; temp_B != NULL; temp_B = temp_B -> next )
		{

			// Make intersection (which is multiplication of 2 cubes) //
			temp_cube = make_intersect ( temp_A -> cube, temp_B -> cube );

			// Check for validation and add it //
			if ( !not_valid_cube ( temp_cube ) )
			{
				cube_list_insert ( result, temp_cube);
			}

		}
	}

	return result;
}

/*	includes_in func:
*
*	Checks if the included function is included in the includer.
*	Returns the rest of includer function if is included, else
*	NULL.
*
*	For example if included is ab and includer a, returns b
*	If included is abc and includer abe returns NULL
*/
char *includes_in ( char *included, char *includer)
{

	int i;
	int length;
	char *rest_string;

	length = strlen ( includer );

	rest_string = ( char *) malloc ( sizeof ( char )* ( length + 1) );

	if ( strlen(included) != length )
		return NULL;

	for ( i = 0; i < length; i = i + 2)
	{
		if ( included[i] == '1' && included[i+1] == '0')
		{
			// Check for example if included is ab' and includer ab or a //
			if ( includer[i] != '1' || includer[i+1] != '0')
			{
				free ( rest_string );
				return NULL;
			}

			/* As this terms belongs both to included and includer, it cannot
			*  belong to rest.
			*/
			else
			{
				rest_string [i]     = '1';
				rest_string [i + 1] = '1';
			}
		}

		if ( included[i] == '0' && included[i+1] == '1')
		{
			// Check for example if included is ab and includer ab' or a //
			if ( includer[i] != '0' || includer[i+1] != '1')
			{
				free ( rest_string );
				return NULL;
			}

			/* As this terms belongs both to included and includer, it cannot
			*  belong to rest.
			*/
			else
			{
				rest_string [i]     = '1';
				rest_string [i + 1] = '1';
			}
		}

		// If term does not belong to included //
		if ( included[i] == '1' && included[i+1] == '1')
		{
			/* Check if belongs to includer and add to rest,
			*  else doesn't belong somewhere.
			*/
			if ( includer[i] == '1' && includer[i+1] == '1')
			{
				rest_string [i]     = '1';
				rest_string [i + 1] = '1';
			}
			else
			{
				rest_string [i]     = includer[i];
				rest_string [i + 1] = includer[i+1];
			}
		}
	}

	rest_string [i] = '\0';
	return rest_string;
}

void division_algebric_fromStr ( char **divisor_str, char **dividend_str, int num_divisor_cubes, int num_dividend_cubes)
{

	unsigned int i;
	cube_list_node *divisor, *dividend, *quotient, *remainder;
	cube_list_node **q_r;

	if ( num_dividend_cubes < 1 || num_divisor_cubes < 1 )
		return;

	divisor = cube_list_initialize();
	for ( i = 0; i < num_divisor_cubes; i++ )
	{
		cube_list_insert ( divisor, divisor_str[i]);
	}

	dividend = cube_list_initialize();
	for ( i = 0; i < num_dividend_cubes; i++ )
	{
		cube_list_insert ( dividend, dividend_str[i]);
	}

	q_r = division_algebric ( divisor, dividend, num_dividend_cubes);

	quotient  = q_r [0];
	remainder = q_r [1];
	free ( q_r );

	printf("Divisor:   %s\n", cube_list_getChar ( divisor ));
	printf("Dividend:  %s\n", cube_list_getChar ( dividend ));
	printf("Quotient:  %s\n", cube_list_getChar ( quotient ));
	printf("Remainder: %s\n", cube_list_getChar ( remainder));

	cube_list_destroy ( divisor );
	cube_list_destroy ( dividend );
	cube_list_destroy ( quotient );
	cube_list_destroy ( remainder );

}

/*	division_algebric func:
*
*	Implements the algebric - weak division algorithm
*	between 2 cube functions. Returns a table of 2 cube functions.
*	At [0] place is the quontient
*	At [1] place is the remainder
*/
cube_list_node **division_algebric ( cube_list_node *divisor, cube_list_node *dividend, int num_dividend_cubes)
{

	int i;
	cube_list_node *temp_A, *temp_B;
	char *temp_cube;

	cube_list_node **quotient_possibles;
	cube_list_node *quotient, *temp_quotient;

	cube_list_node *quotient_dividend;

	cube_list_node *remainder;

	cube_list_node **return_table;

	quotient_possibles = ( cube_list_node* *) malloc ( sizeof ( cube_list_node* ) * num_dividend_cubes );

	// Run accross the divindend function //
	i = 0;
	for ( temp_A = dividend -> next; temp_A != NULL; temp_A = temp_A -> next )
	{

		quotient_possibles[i] = cube_list_initialize();
		
		// Run accross the divisor function //
		for ( temp_B = divisor -> next; temp_B != NULL; temp_B = temp_B -> next )
		{
			// If the dividend is included in the divisor, add it in possible q //
			temp_cube = includes_in ( temp_A -> cube, temp_B -> cube );
			if ( temp_cube != NULL )
			{
				cube_list_insert ( quotient_possibles[i], temp_cube);
				free ( temp_cube );
			}
		}

		i++;
	}

	// Calculate the intersection of the possible q's which is the quontient //
	quotient = quotient_possibles [0];
	for ( i = 1; i < num_dividend_cubes; i++ )
	{
		temp_quotient = cube_functions_intersection ( quotient, quotient_possibles [i] );
		cube_list_destroy ( quotient );
		quotient = temp_quotient;
	}
	free ( quotient_possibles );

	// Make Q*P //
	quotient_dividend = multiplication ( quotient, dividend );

	// Find R = F - Q*R which is the remainder //
	remainder = cube_list_initialize();
	for ( temp_A = divisor -> next; temp_A != NULL; temp_A = temp_A -> next )
	{
		if ( cube_list_search_node ( quotient_dividend, temp_A -> cube ) == NULL )
		{
			cube_list_insert ( remainder, temp_A -> cube );
		}
	}

	// Make the return table and add quontient and remainder //
	return_table = ( cube_list_node* *) malloc ( sizeof ( cube_list_node* )*2 );
	return_table[0] = quotient;
	return_table[1] = remainder;

	return return_table;



}
