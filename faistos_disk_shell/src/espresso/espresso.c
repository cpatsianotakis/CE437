#include "espresso.h"

cube_list_node *expand ( cube_list_node *f_ON, cube_list_node *f_DC )
{

	int i, j, min;

	cube_list_node *temp;
	cube_list_node *f_OFF;
	cube_list_node *new_f;
	cube_list_node *on_union_dc;

	char *intersected_cube, *expanded_cube;

	int cube_length, cube_nums, cube_nums_new;
	int *table_CSV, *table_weight;

	cube_nums = cube_list_contents( f_ON );

	if ( cube_nums < 1)
		return NULL;

	temp = f_ON -> next;
	cube_length = strlen ( temp -> cube );

	table_CSV = (int *)malloc ( sizeof ( int ) * cube_length);
	if ( table_CSV == NULL )
		return NULL;

	for ( i = 0; i < cube_length; i++ )
		table_CSV[i] = 0;

	table_weight = (int *)malloc ( sizeof ( int ) * cube_nums);
	if ( table_weight == NULL )
		return NULL;
	
	for ( i = 0; i < cube_nums; i++ )
		table_weight [i] = 0;

	// Find the union of ON SET and DC SET to find OFF SET //
	on_union_dc = cube_functions_union ( f_ON, f_DC );

	// Find f_OFF and the number of cubes it has //
	f_OFF = get_OFF_f ( on_union_dc );

	// Destroy the no more needed list (function) //
	cube_list_destroy ( on_union_dc );

	// Make the function that is going to be returned //
	new_f = cube_list_initialize();
	if ( new_f == NULL )
		return NULL;
	for ( temp = f_ON -> next; temp != NULL; temp = temp -> next)
	{
		if ( cube_list_insert ( new_f, temp -> cube) == 0 )
			return NULL;
	}

	// Make the CSV table //
	for ( i = 0; i < cube_length; i++)
	{
		for ( temp = new_f -> next; temp != NULL; temp = temp -> next)
			if ( temp -> cube[i] == '1')
				table_CSV[i]++;
	}

	// Find weight of each cube //
	for ( temp = new_f -> next, j = 0; temp != NULL; temp = temp -> next, j++)
	{
		for ( i = 0; i < cube_length; i++ )
		{
			if ( temp -> cube[i] == '1')
				table_weight[j] = table_weight[j] + table_weight[i];
		}
	}

	for ( i = 0; i < cube_nums; i++ )
	{
		// Find the cube with min weight of f_ON //
		cube_nums_new = cube_list_contents ( new_f );
		min = 0;
		for ( j = 0; j < cube_nums_new; j++)
		{
			if ( ( table_weight [j] < table_weight [min] ) && ( table_weight [j] != -1 ) )
				min = j;
		}

		for ( temp = new_f -> next, j = 0; temp != NULL; temp = temp -> next, j++)
		{
			if ( j == min)
				break;
		}
		expanded_cube = temp -> cube;

		// If the cube is not targeted as covered //
		if ( expanded_cube [0] != '2'  )
		{

			for ( j = 0; j < cube_length; j++ )
			{
				if ( expanded_cube[j] == '0')
				{
					expanded_cube [j] = '1';

					// Check intersection with the OFF set //
					for ( temp = f_OFF -> next; temp != NULL; temp = temp -> next )
					{
						intersected_cube = make_intersect ( temp -> cube, expanded_cube );
						if ( intersected_cube == NULL )
							return NULL;

						// If is the intersection with Off set is valid, no acceptable //
						if ( !not_valid_cube( intersected_cube) )
						{
							expanded_cube [ j ] = '0';
						}
					}

				}
			}

			//Set the first char of cube string as 2, in order to target the covered cubes //
			for ( temp = new_f -> next; temp != NULL; temp = temp -> next)
			{
				if ( ( temp -> cube [0] != '2' ) && ( check_coverage ( expanded_cube, temp -> cube ) > 0 ) )
				{
					temp -> cube [0] = '2';
				}
			}

		}

		// Target as passed //
		table_weight [ min ] = -1;

	}

	// Clean the function of covered cubes //
	for ( temp = new_f -> next; temp != NULL; temp = temp -> next)
	{
		if ( temp -> cube[0] == '2' )
		{
			temp = temp -> prev;
			if ( cube_list_remove ( new_f, temp -> cube ) == 1 )
				return NULL;
		}
	}

	// Free the no more needed tables //
	free ( table_weight );
	free ( table_CSV );

	return new_f;

}

cube_list_node *reduce ( cube_list_node *f_ON, cube_list_node *f_DC )
{

	cube_list_node *on_union_dc, *new_f;
	cube_list_node *set_Q, *set_Q_OFF, *set_Q_OFF_cube;
	cube_list_node *temp_A, *temp_B;

	char *temp_cube, *supercube;

	on_union_dc = cube_functions_union ( f_ON, f_DC );
	new_f = cube_functions_union ( f_ON, NULL );

	for ( temp_A = new_f -> next; temp_A != NULL; temp_A = temp_A -> next )
	{

		set_Q = cube_list_initialize();
		set_Q_OFF_cube = cube_list_initialize();

		// Calculate Q //
		for ( temp_B = on_union_dc -> next; temp_B != NULL; temp_B = temp_B -> next)
		{
			if ( strcmp (temp_B -> cube, temp_A -> cube ) != 0 )
				cube_list_insert ( set_Q, temp_B -> cube );
		}

		// Calculate Q' //
		set_Q_OFF = get_OFF_f ( set_Q );

		// Calculate Q'|a //
		for ( temp_B = set_Q_OFF -> next; temp_B != NULL; temp_B = temp_B -> next)
		{
			temp_cube = make_intersect ( temp_B -> cube, temp_A -> cube);
			if ( !not_valid_cube ( temp_cube ) )
			{
				cube_list_insert ( set_Q_OFF_cube, temp_cube);
			}
			free ( temp_cube );
		}

		// Calculate supercube ( Q'|a ) //
		supercube = set_Q_OFF_cube -> next -> cube;
		for ( temp_B = set_Q_OFF_cube -> next; temp_B != NULL; temp_B = temp_B -> next )
		{
			temp_cube = make_supercube ( supercube, temp_B -> cube );
			free ( supercube );
			supercube = temp_cube;
		}

		// Calculate a~ = a (intersect) supercube ( Q'|a ) // 
		temp_cube = make_intersect ( supercube, temp_A -> cube );
		free ( temp_A -> cube );
		temp_A -> cube = temp_cube;

		cube_list_destroy ( set_Q_OFF_cube );
		cube_list_destroy ( set_Q_OFF );
		cube_list_destroy ( set_Q );

	}
	
	return new_f;
}

cube_list_node *irredundant ( cube_list_node *f_ON, cube_list_node *f_DC )
{
	int i, j;
	int cube_nums;

	int *Rp_final_intList;
	char *temp_cube;

	cube_list_node *temp_A, *temp_B, *temp_C;

	cube_list_node *set_Er, *set_Rt, *set_Rp, *set_H, *set_H_cube;
	cube_list_node *on_union_dc, *dc_union_Er, *dc_union_Er_union_Rp;
	cube_list_node *final_list;

	row_ucp *ucp_table;


	// Find the union of ON SET and DC SET to find OFF SET //
	on_union_dc = cube_functions_union ( f_ON, f_DC );

	set_Er = cube_list_initialize();
	set_Rp = cube_list_initialize();
	set_Rt = cube_list_initialize();

	// Make Er set //
	for ( temp_A = f_ON -> next; temp_A != NULL; temp_A = temp_A -> next )
	{
		cube_list_insert ( set_Er, temp_A -> cube);

		for ( temp_B = on_union_dc -> next; temp_B != NULL; temp_B = temp_B -> next )
		{

			if ( strcmp ( temp_A -> cube , temp_B -> cube) != 0 )
			{
				if ( check_coverage ( temp_B -> cube , temp_A -> cube ) > 0 )
				{
					cube_list_remove ( set_Er, temp_A -> cube);
				}
			}

		}

	}

	// Find the union of DC SET and Er SET to find the Rp SET //
	dc_union_Er = cube_functions_union ( set_Er, f_DC );

	// Make Rt set //
	for ( temp_A = f_ON -> next; temp_A != NULL; temp_A = temp_A -> next )
	{
		if ( cube_list_search_node ( set_Er , temp_A -> cube) == NULL )
		{

			for ( temp_B = dc_union_Er -> next; temp_B != NULL; temp_B = temp_B -> next )
			{

				if ( check_coverage ( temp_B -> cube , temp_A -> cube ) > 0 )
				{
					cube_list_insert ( set_Rt, temp_A -> cube);
				}

			}
		}
	}

	// Make Rp set //
	for ( temp_A = f_ON -> next; temp_A != NULL; temp_A = temp_A -> next )
	{
		if ( cube_list_search_node ( set_Er , temp_A -> cube) == NULL && cube_list_search_node ( set_Rt , temp_A -> cube) == NULL )
		{
			cube_list_insert ( set_Rp, temp_A -> cube );
		}
	}


	// Find the union of DC SET and Er SET and Rp SET to find H set //
	dc_union_Er_union_Rp = cube_functions_union ( dc_union_Er , set_Rp );

	ucp_table = initialize_ucp_table ( cube_list_contents ( set_Rp), cube_list_contents ( set_Rp ) );

	for ( temp_C = set_Rp -> next, i = 0; temp_C != NULL; temp_C = temp_C -> next, i++ )
	{
		
		// MAKE THE H SET
		set_H = cube_list_initialize();
		for ( temp_A = dc_union_Er_union_Rp -> next; temp_A != NULL; temp_A = temp_A -> next )
		{
			cube_list_insert ( set_H, temp_A -> cube);

			for ( temp_B = on_union_dc -> next; temp_B != NULL; temp_B = temp_B -> next )
			{

				if ( strcmp ( temp_A -> cube , temp_B -> cube) != 0 )
				{
					if ( check_coverage ( temp_B -> cube , temp_A -> cube ) > 0 )
					{
						cube_list_remove ( set_H, temp_A -> cube);
					}
				}

			}

		}

		for ( temp_A = set_Rp -> next, j = 0; temp_A != NULL; temp_A = temp_A -> next , j++)
		{

			temp_cube = make_intersect ( temp_A -> cube, temp_C -> cube );
			if ( not_valid_cube ( temp_cube ) )
			{
				ucp_table [i]. row_ptr [j] = 0;
			}
			else
			{
				ucp_table [i]. row_ptr [j] = 0;
			}
			free ( temp_cube );
		}

		cube_list_destroy ( set_H );
	}

	Rp_final_intList = ucp_lines ( ucp_table, NULL, cube_list_contents ( set_Rp), cube_list_contents ( set_Rp), 1 );

	final_list = cube_functions_union ( set_Er, NULL );

	for ( i = 0; Rp_final_intList[i] != -1; i++ )
	{
		for ( j = 0, temp_A = set_Rp -> next; j < Rp_final_intList [i]; temp_A = temp_A -> next, j++ );

			cube_list_insert ( final_list, temp_A -> cube );
	}

	cube_list_destroy ( set_Er );
	cube_list_destroy ( set_Rt );
	cube_list_destroy ( set_Rp );
	cube_list_destroy ( on_union_dc );
	cube_list_destroy ( dc_union_Er );
	cube_list_destroy ( dc_union_Er_union_Rp );
}

int calculate_cost ( cube_list_node *f )
{

	int cube_size;
	int i;
	int final_cost = 0;
	cube_list_node *temp;

	if ( f -> next == NULL )
		return -1;

	if ( not_valid_cube ( f -> next -> cube ))
		return -1;

	cube_size = strlen ( f -> next -> cube );

	for ( temp = f -> next; temp != NULL; temp = temp -> next )
	{
		for ( i = 0; i < cube_size; i = i + 2 )
		{
			if ( temp -> cube [i] == 0 || temp -> cube [i + 1] == 0 )
				final_cost++;
		}
	}

	return final_cost;
}

cube_list_node *espresso ( cube_list_node *f_ON, cube_list_node *f_DC)
{

	cube_list_node *f_final, *f_temp;
	int cost, next_cost;

	f_final = expand ( f_ON, f_DC );

	f_temp  = irredundant ( f_final, f_DC );
	cube_list_destroy ( f_final );
	f_final = f_temp;

	next_cost = calculate_cost ( f_final );

	do
	{
		cost = next_cost;

		f_temp = reduce ( f_ON, f_DC );
		cube_list_destroy ( f_final );
		f_final = f_temp;

		f_temp = expand ( f_ON, f_DC );
		cube_list_destroy ( f_final );
		f_final = f_temp;

		f_temp  = irredundant ( f_final, f_DC );
		cube_list_destroy ( f_final );
		f_final = f_temp;

		next_cost = calculate_cost ( f_final );
	} 
	while ( next_cost < cost);


}
