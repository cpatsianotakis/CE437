# include "espresso.h"

row_ucp *initialize_ucp_table ( int rows, int cols )
{

	int i;
	row_ucp *new_table;

	new_table = ( row_ucp *)malloc ( sizeof ( row_ucp ) * rows);

	for ( i = 0; i < rows; i++ )
	{

		new_table[i]. row_length = cols;
		new_table[i]. row_ptr = ( int *) malloc ( sizeof ( int ) * cols);
	}

	return new_table;
}

row_ucp *make_ucp_table ( int **table, int rows, int cols )
{

	int i, j;
	row_ucp *new_table;

	new_table = ( row_ucp *)malloc ( sizeof ( row_ucp ) * rows);

	for ( i = 0; i < rows; i++ )
	{

		new_table[i]. row_length = cols;
		new_table[i]. row_ptr = ( int *) malloc ( sizeof ( int ) * cols);

		for ( j = 0; j < cols; j++ )
		{
			new_table[i]. row_ptr [j] = table[i][j];
		}

	}

	return new_table;
}

void destroy_ucp_table ( row_ucp *table, int rows )
{

	int i;

	for ( i = 0; i < rows; i++ )
	{
		free ( table -> row_ptr );
	}

	free ( table );
}

void remove_row ( row_ucp *table, int row, int *rows_num, int *cols_num )
{

	int i, j, k;

	for ( i = 0; i < ( table -> row_length ); i++ )
	{

		if ( table -> row_ptr[i] == 1)
		{

			for ( j = 0; j < *rows_num; j++ )
			{
				for ( k = i; k < (*cols_num - 1) ; k++ )
				{
					table [j]. row_ptr [k] = table [j]. row_ptr [k + 1];
				}
			}

			*cols_num = *cols_num - 1;
			i--;

		}
	}

	for ( i = row; i < ( *rows_num - 1 ); i++ )
	{
		table [i] = table [i+1];
	}

	*rows_num = *rows_num - 1;

}

int *ucp_lines ( row_ucp *row_ucp_table, int **int_table, int rows, int cols, int MODE)
{

	int i, j, k;
	int cols_left, rows_left;
	int ones_at_row, covered, covers, remove_times, max_ones, max_ones_place;
	int targeted;

	row_ucp *internal_table;

	int essential_cubes [ rows ];
	int essential_cubes_ptr = 0;
	int *essential_cubes_alloc;

	cols_left = cols;
	rows_left = rows;

	if ( MODE == 0)
		internal_table = make_ucp_table ( int_table, rows, cols );
	else
		internal_table =  row_ucp_table;

	while ( cols_left > 0 )
	{
		remove_times = 0;


		// APARAITITES STILES //
		for ( i = 0; i < cols_left; i++ )
		{

			ones_at_row = 0;
			for ( j = 0; j < rows_left; j++ )
			{
				if( internal_table [j]. row_ptr[i] == 1 )
				{
					ones_at_row++;
					targeted = j;
				}
			}

			if ( ones_at_row == 1 )
			{
				essential_cubes [ essential_cubes_ptr ] = targeted;
				essential_cubes_ptr++;

				remove_row (internal_table, targeted, &rows_left, &cols_left);
				i = -1;

				remove_times = 1;

			}
		}


		// APALOIFI KATA SEIRA //
		for ( i = 0; i < cols_left; i++ )
		{

			covers = 0;
			for ( j = 0; j < cols_left; j++ )
			{
				covered = 1;
				for ( k = 0; k < rows_left; k++)
				{
					if( ( internal_table [j]. row_ptr[k] == 1 ) && ( internal_table [i]. row_ptr[k] == 0 )  )
					{
						covered = 0;
					}
				}

				if ( covered == 1 )
				{
					remove_row (internal_table, j, &rows_left, &cols_left);
					covers = 1;
				}
			}

			if ( covers == 1)
			{
				essential_cubes [ essential_cubes_ptr ] = i;
				essential_cubes_ptr++;
				remove_row (internal_table, i, &rows_left, &cols_left);
				i = -1;

				remove_times = 1;
			}
			
		}

		// APALOIFI KATA STILI //
		for ( i = 0; i < rows_left; i++ )
		{

			for ( j = 0; j < rows_left; j++ )
			{
				covered = 1;
				for ( k = 0; k < cols_left; k++)
				{
					if( ( internal_table [k]. row_ptr[j] == 1 ) && ( internal_table [k]. row_ptr[i] == 0 )  )
					{
						covered = 0;
					}
				}

				if ( covered == 1 )
				{

					for ( k = 0; k < rows_left; k++ )
					{
						if( internal_table [k]. row_ptr[j] == 1 )
						{
							essential_cubes [ essential_cubes_ptr ] = k;
							essential_cubes_ptr++;
							remove_row (internal_table, k, &rows_left, &cols_left);

							remove_times = 1;
						}
					}

				}
			}
			
		}


		if ( remove_times == 0 )
		{

			max_ones = 0;
			max_ones_place = 0;
			for ( i = 0; i < rows_left; i++ )
			{

				k = 0;
				for ( j = 0; j < cols_left; j++ )
				{
					if ( internal_table [i]. row_ptr [j] == 1 )
						k++;
				}
				if ( max_ones < k )
					max_ones_place = i;

			}

			essential_cubes [ essential_cubes_ptr ] = max_ones_place;
			essential_cubes_ptr++;
			remove_row (internal_table, max_ones_place, &rows_left, &cols_left);
		}

	}

	essential_cubes_alloc = ( int *) malloc ( sizeof ( int )* essential_cubes_ptr + 1);
	for ( i = 0; i < essential_cubes_ptr; i++ )
	{
		essential_cubes_alloc[i] = essential_cubes[i];
	}
	essential_cubes_alloc[i] = -1;

	destroy_ucp_table ( internal_table, rows );

	return essential_cubes_alloc;
}