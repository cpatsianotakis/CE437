#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "graph.h"

void graph_item_table_initialize ()
{
	if ( GRAPH_TABLE != NULL)
		graph_item_table_destroy();

	GRAPH_SIZE = 0;
	GRAPH_TABLE = NULL;
}

void graph_item_table_destroy()
{

	int i;
	graph_node_dist *temp;

	for ( i = 0; i < GRAPH_SIZE; i++)
	{

		temp = GRAPH_TABLE[i].head_neighboors;

		while ( temp != NULL )
		{

			GRAPH_TABLE[i].head_neighboors = temp -> next;
			free ( temp );
			temp = GRAPH_TABLE[i].head_neighboors;
		}
		free ( GRAPH_TABLE[i].vertic_name );
	}

	free ( GRAPH_TABLE );

	GRAPH_TABLE = NULL;
	GRAPH_SIZE = 0;

}

int graph_item_table_add_vertic ( char *vertic_name )
{

	GRAPH_TABLE = realloc ( GRAPH_TABLE, sizeof( graph_item ) * ( GRAPH_SIZE + 1) );
	if ( GRAPH_TABLE == NULL)
		return -1;

	GRAPH_TABLE [ GRAPH_SIZE ].index = GRAPH_SIZE;
	GRAPH_TABLE [ GRAPH_SIZE ].vertic_name = strdup ( vertic_name );
	GRAPH_TABLE [ GRAPH_SIZE ].head_neighboors = NULL;

	GRAPH_SIZE++;
	
	return 0;
}

int graph_item_table_getindex ( char *vertic_name)
{

	int i;

	for ( i = 0 ; i < GRAPH_SIZE; i++)
	{
		if ( strcmp ( GRAPH_TABLE[i].vertic_name, vertic_name) == 0 )
			return i;
	}

	return -1;
}

void graph_item_table_printvertic()
{
	int i;

	if ( GRAPH_TABLE == NULL)
		return;

	for ( i = 0; i < GRAPH_SIZE; i++ )
	{
		printf("%s\n", GRAPH_TABLE[i].vertic_name);
	}
}

int graph_node_dist_add ( char *source_vertic_name, int drain_vertic_index, int distance)
{

	int source_vertic_index;

	graph_node_dist *new_node;

	source_vertic_index = graph_item_table_getindex (source_vertic_name);
	if ( source_vertic_index == -1)
		return -1;

	new_node = malloc ( sizeof ( graph_node_dist *) );
	if ( new_node == NULL )
		return -1;

	new_node->next = GRAPH_TABLE [ source_vertic_index ].head_neighboors;
	GRAPH_TABLE [ source_vertic_index ].head_neighboors = new_node;

	new_node -> index = drain_vertic_index;
	new_node -> distance = distance;

	return 0;
}

int graph_index_deque ( int table [], int *current_size)
{
	int i;
	int return_value;

	return_value = table[0];

	for ( i = 0; i < *current_size; i++)
	{	
		table[i] = table[ i + 1 ];
	}

	*current_size = *current_size - 1;
	return return_value ;
}

void graph_index_enque ( int table [], int *current_size, int new_value)
{

	table[ *current_size ] = new_value;
	*current_size = *current_size + 1;
}


