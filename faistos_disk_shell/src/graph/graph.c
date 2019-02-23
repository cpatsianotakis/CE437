#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "graph.h"

//   ===== decode_line function =====   //
/*	Decodes a string which has prototype
*	"NodeSrc -> NodeDrain Dist"
*	Makes new nodes if the referenced nodes does not excist
*	and makes the edge 
*/
int decode_line ( char *line_str, int line )
{

	char *source_vertic_name, *drain_vertic_name;
	char *whitespace;
	char *dist_str;

	int drain_index, distance;
	
	// Find first space where NodeSrc name ends //
	whitespace = strchr ( line_str, ' ');
	if ( whitespace == NULL )
	{
		printf("Error: ");
		printf(ANSI_COLOR_RED "(Line: %d) " ANSI_COLOR_RESET, line );  
		printf("Input cannot be one word!\n");
		return 2;
	}
	*whitespace = '\0';

	// Add NodeSrc if doesn't excist //
	source_vertic_name = line_str;
	if ( graph_item_table_getindex ( source_vertic_name ) == -1 )
	{
		graph_item_table_add_vertic ( source_vertic_name );
	}

	while ( *(whitespace + 1) == ' ')
		whitespace++;
	
	// Find "->" //
	if ( *(whitespace + 1) != '-' || *(whitespace + 2) != '>')
	{
		printf("Error: ");
		printf(ANSI_COLOR_RED "(Line: %d) " ANSI_COLOR_RESET, line ); 
		printf("\"->\" Not found!\n");
		return 2;
	}

	whitespace = strchr ( whitespace + 3 , ' ');
	if ( whitespace == NULL )
	{
		printf("Error: ");
		printf(ANSI_COLOR_RED "(Line: %d) " ANSI_COLOR_RESET, line ); 
		printf("No whitespace found after \" -> \"!\n");
		return 2;
	}

	while ( *(whitespace + 1) == ' ')
		whitespace++;

	// Find NodeDrain //
	drain_vertic_name = whitespace + 1;
	whitespace = strchr ( drain_vertic_name, ' ');
	if ( whitespace == NULL )
	{
		printf("Error: ");
		printf(ANSI_COLOR_RED "(Line: %d) " ANSI_COLOR_RESET, line ); 
		printf("No whitespace found after drain vertic\n");
		return 2;
	}
	*whitespace = '\0';

	// Put it in table if doesn't excist //
	if ( (drain_index = graph_item_table_getindex ( drain_vertic_name ) ) == -1 )
	{
		graph_item_table_add_vertic ( drain_vertic_name );
		drain_index = graph_item_table_getindex ( drain_vertic_name );
	}

	while ( *(whitespace + 1) == ' ')
	{
		whitespace++;
	}

	// Find distance and make edge //
	dist_str = whitespace + 1;
	whitespace = strchr ( dist_str, ' ');
	if ( whitespace != NULL )
	{
		whitespace = '\0';
	}

	distance = atoi ( dist_str );

	graph_node_dist_add ( source_vertic_name, drain_index, distance);

	return 0;
}

//   ===== create_graph_from_file function =====   //
/*	Gets a file and makes the graph table
*/
int create_graph_from_file ( char *file_path )
{

	int fd;
	int file_size;

	int line = 0;

	char *file_chars, *file_chars_ptr;
	char *enter_char;

	graph_item_table_initialize();



	// Get the file as a string //

	fd = open ( file_path, O_RDONLY, S_IRWXU);
	if ( fd < 0 )
		return -1;
	
	// find size //
	file_size = lseek ( fd, 0, SEEK_END);
	if ( file_size <= 0)
		return -1;

	file_chars = ( char *)malloc ( ( file_size + 1) * sizeof( char ) );
	if ( file_chars == NULL )
	{
		return -1;
	}
	
	if ( lseek ( fd, 0, SEEK_SET) < 0)
		return -1;
	
	if ( read ( fd, file_chars, file_size) < 0 )
		return -1;
	
	file_chars [ file_size ] = '\0';



	// Decode for each line //
	file_chars_ptr = file_chars;
	while ( ( enter_char = strchr ( file_chars_ptr, '\n') ) != NULL )
	{
		*enter_char = '\0';
		if ( decode_line ( file_chars_ptr, line ) != 0)
			return 2;

		line++;

		file_chars_ptr = enter_char + 1;
	}
	if ( decode_line ( file_chars_ptr, line ) != 0)
		return 2;


	free ( file_chars);

	close ( fd );

	return 0;	
}

//   ===== node_source_to_file function =====   //
/*	For a node of graph table, it prints the edges which start from it
*	to a file. The file is given with the opened file descriptor
*/
int node_source_to_file ( graph_item item, int fd)
{

	graph_node_dist *temp;

	char to_file_string [100];
	int to_file_string_length;

	for ( temp = item.head_neighboors ; temp != NULL;  temp = temp->next)
	{
		// Make string //
		sprintf ( to_file_string, "%s -> %s %d\n", item.vertic_name, GRAPH_TABLE [ temp -> index ].vertic_name, temp -> distance);
		
		// Find string length for write function //
		to_file_string_length = strlen ( to_file_string );
		write ( fd, to_file_string,  to_file_string_length);
	}

	return 0;
}

//   ===== write_graph_to_file function =====   //
/*	Writes the graph table to a file
*/
int write_graph_to_file ( char *file_name )
{

	int fd;
	int i;

	if ( GRAPH_SIZE == 0)
	{
		return 1;
	}

	fd = open ( file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if ( fd < 0 )
	{
		return -1;
	}

	for( i = 0 ; i < GRAPH_SIZE; i++ )
	{
		node_source_to_file ( GRAPH_TABLE[i], fd );
	}

	close ( fd );

	return 0;
}

//   ===== set_inputs function =====   //
/*	Finds the inputs for each edge of graph table
*	The function is void, but changes the values
*	of a table that is given in input.
*/
void set_inputs (int table[])
{
	
	int check_vertic, j;
	graph_node_dist *temp;

	int inputs;
	
	// A for loop to go across the inputs table //
	for ( check_vertic = 0; check_vertic < GRAPH_SIZE; check_vertic++)
	{
		// check_vertic the vertic that is going to check it's inputs //
		inputs = 0;

		// A for loop to go across the graph table to find edges that have destination the check_vertic //
		for ( j = 0; j < GRAPH_SIZE; j++)
		{
			
			// Go accross the vertices //
			for ( temp = GRAPH_TABLE[ j ].head_neighboors; temp != NULL; temp = temp -> next)
			{

				if ( temp -> index == check_vertic )
					inputs++;
			}

		}
		table[ check_vertic] = inputs;

	}
}

//   ===== set_outputs function =====   //
/*	Finds the outputs for each edge of graph table
*	The function is void, but changes the values
*	of a table that is given in input.
*/
void set_outputs (int table[])
{
	
	int check_vertic;
	graph_node_dist *temp;

	int outputs;

	// Go across the graph table to check the vertices //
	for ( check_vertic = 0; check_vertic < GRAPH_SIZE; check_vertic++)
	{
		outputs = 0;

		for ( temp = GRAPH_TABLE[ check_vertic ].head_neighboors; temp != NULL; temp = temp -> next)
		{
			outputs++;
		}

		table[ check_vertic] = outputs;

	}
}

//   ===== make_distances function =====   //
/*	Finds the maximum distance for each vertic
*	from the sources
*/
void make_distances ( int dist_table [] )
{

	int inputs [ GRAPH_SIZE ];

	int fifo[ GRAPH_SIZE ];
	int fifo_size = 0;

	graph_node_dist *temp;

	int i, current_vertic;
	
	// Make inputs //
	set_inputs ( inputs );

	// Initialize to zero //
	for ( i = 0; i < GRAPH_SIZE; i++)
	{
		dist_table[i] = 0;
	}

	// Enque to fifo the sources //
	for ( i = 0; i < GRAPH_SIZE; i++)
	{
		if ( inputs[i] == 0)
			graph_index_enque ( fifo, &fifo_size, i);
	}

	while ( fifo_size > 0)
	{
		// Get next
		current_vertic = graph_index_deque ( fifo, &fifo_size );

		for ( temp = GRAPH_TABLE [ current_vertic ].head_neighboors; temp != NULL; temp = temp -> next)
		{
			// Check curr distance with the max and change the max if is less
			if ( ( ( temp -> distance ) + dist_table [current_vertic] ) > dist_table [ temp -> index ])
			{
				dist_table [ temp -> index ] = ( temp -> distance ) + dist_table [current_vertic];
			}

			inputs [ temp -> index ]--;

			// If the drain vertic has no more input, enque it
			if ( inputs [ temp -> index ] == 0)
			{
				graph_index_enque ( fifo, &fifo_size, temp -> index);
			}

		}


	}

}

//   ===== longest_path function =====   //
/*	Finds the longest path and saves it in the argument
*	longest_path_table and returns the size of it. 
*/
int longest_path (int longest_path_table [], int dist_table [] )
{

	int outputs [ GRAPH_SIZE ];

	int i;
	int drain_longest_path = 0;
	int drain_longest_path_next;
	int longest_path_size = 0;
	int max_dist;

	graph_node_dist *temp;

	// Find Drains of the graph //
	set_outputs ( outputs );
	
	// Find the drain which has the longest path //
	max_dist = 0;
	for ( i = 0; i < GRAPH_SIZE; i++)
	{

		if ( outputs[i] == 0 )
		{
			if ( max_dist < dist_table [ i ])
			{
				drain_longest_path = i;
				max_dist = dist_table [ drain_longest_path ];
			}
		}
	}


	longest_path_table[0] = drain_longest_path;
	longest_path_size++;

	do{
		max_dist = -1; //-1 to get source vertic //
		drain_longest_path_next = -1;

		for ( i = 0; i < GRAPH_SIZE; i++){

			for ( temp = GRAPH_TABLE [ i ].head_neighboors; temp != NULL; temp = temp -> next)
			{
				// Searching for edges to current longest path //
				if ( temp -> index == drain_longest_path )
				{
					if ( max_dist < dist_table [ i ])
					{
						drain_longest_path_next = i;
						max_dist = dist_table [ i ];
	
					}
		
				}
			}

		}
		
		// If there is found a previous vertic in path // 
		if ( drain_longest_path_next != -1)
		{
			drain_longest_path = drain_longest_path_next;
			longest_path_table [ longest_path_size ] = drain_longest_path;
			longest_path_size++;
		}

	}while ( drain_longest_path_next != -1);

	return longest_path_size;
}

//   ===== decode_line function =====   //
/*	Decodes a string which has prototype
*	"NodeSrc -> NodeDest Dist"
*	Makes new nodes if the referenced nodes does not excist
*	and makes the edge 
*/
void get_slack ( int max_legal_dist, int slack [], int dist_table [] )
{

	int outputs [ GRAPH_SIZE ];

	int fifo [ GRAPH_SIZE ];
	int fifo_size = 0;

	int max_dist = 0;

	int current_vertic;
	int i;
	graph_node_dist *temp;

	set_outputs ( outputs );
	for ( i = 0; i < GRAPH_SIZE; i++)
	{
		if ( outputs[i] == 0 )
		{
			graph_index_enque ( fifo, &fifo_size, i);

			if ( max_dist < dist_table [i] )
				max_dist = dist_table [i];
		}
	}

	for ( i = 0; i < GRAPH_SIZE; i++)
	{
		// If vertic is output, set slack the correct value //
		if ( outputs [ i ] == 0)
			slack [ i ] = max_legal_dist - dist_table [ i ];

		// Just put a value bigger than it could be, in order to get in condition //
		else
			slack [ i ] = max_dist;
	}

	while ( fifo_size > 0)
	{

		current_vertic = graph_index_deque ( fifo, &fifo_size ); 

		for ( i = 0; i < GRAPH_SIZE; i++){

			for ( temp = GRAPH_TABLE [ i ].head_neighboors; temp != NULL; temp = temp -> next)
			{
				if ( temp -> index == current_vertic )
				{

					if ( ( slack [ i ]  ) > slack [ current_vertic ] + ( dist_table [ current_vertic ] - ( dist_table [ i ] + temp -> distance ) ) )
					{
						slack[i] = slack [ current_vertic ] + ( dist_table [ current_vertic ] - ( dist_table [ i ] + temp -> distance ) );	
					}

					outputs [ i ]--;

					if ( outputs [ i ] == 0 )
						graph_index_enque ( fifo, &fifo_size, i );
				}
			}

		}

	}

}

//   ===== decode_line function =====   //
/*	Decodes a string which has prototype
*	"NodeSrc -> NodeDest Dist"
*	Makes new nodes if the referenced nodes does not excist
*	and makes the edge 
*/
char *make_critical_path_and_slack ( int max_dist )
{

	int longest_path_table [ GRAPH_SIZE ];
	int longest_path_size;

	int slack [ GRAPH_SIZE ];
	int dist_table [ GRAPH_SIZE ];

	int min_slack;

	int i, local_distance, total_distance;
	char *cur_ver, *next_ver;
	graph_node_dist *temp;

	if ( GRAPH_SIZE == 0)
		return NULL;
	
	make_distances ( dist_table );

	longest_path_size = longest_path ( longest_path_table, dist_table);

	get_slack ( max_dist, slack, dist_table );

	min_slack = slack[ longest_path_table[0] ];

	printf("//=======================//\n");
	printf("  SLACK: %d\n", min_slack);
	printf("\n\n");
	
	printf("//=======================//\n");
	printf("  CRITICAL PATH:\n\n");

	for ( i = 0; i < longest_path_size -1; i++)
	{
		for ( temp = GRAPH_TABLE [ longest_path_table[ i + 1]].head_neighboors; temp != NULL; temp = temp -> next)
		{
			if ( temp -> index == longest_path_table[ i ])
			{
				local_distance = temp -> distance;
				break;
			}
		}

		cur_ver = GRAPH_TABLE [ longest_path_table[ i + 1]].vertic_name;
		next_ver = GRAPH_TABLE [ longest_path_table[ i ]].vertic_name;
		total_distance = dist_table [longest_path_table[ i ]];
		printf("  %s -> %s (Local distace: %d, Total distance: %d)\n", cur_ver, next_ver, local_distance, total_distance);
	}
	printf("\n\n");

	printf("//=======================//\n");
	printf("  SLACK VALUES :\n\n");

	for ( i = 0; i < GRAPH_SIZE; i++)
	{
		printf("  V: %s slack: %d\n", GRAPH_TABLE[i].vertic_name, slack[i]);
	}
	
	return NULL;

}

//   ===== draw_graphiz function =====   //
/*	Makes an output file printing the graph table
*	that the graphiz software can read and draw the graph
*/
int draw_graphiz ( char *file_name )
{

	int fd;
	int i;

	graph_node_dist *temp;

	char to_file_string[100];
	int to_file_string_length;

	if ( GRAPH_SIZE == 0)
	{
		return 1;
	}

	fd = open ( file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if ( fd < 0 )
	{
		return -1;
	}

	write ( fd, "digraph {\n\nnode [fillcolor = \"lightgray\" , shape = \"egg\" frontsize = 10 ]\n\n", 75);

	for( i = 0 ; i < GRAPH_SIZE; i++ )
	{	

		for ( temp = GRAPH_TABLE[i].head_neighboors; temp != NULL; temp = temp -> next){
			
			sprintf ( to_file_string, "%s -> %s [label =\"%d\"]\n", GRAPH_TABLE [ i ].vertic_name, GRAPH_TABLE [ temp ->index ].vertic_name, temp -> distance);
			to_file_string_length = strlen ( to_file_string );
			write ( fd, to_file_string, to_file_string_length);
		}
	}

	write ( fd, "}", 1);
	close ( fd );

	return 0;
}
