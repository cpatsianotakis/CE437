#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cubes.h"

/*	cube_list_contents func:
*
*	Calculates the contents the list has 
*
*	Returns if succes the num of contents, else -1
*/
int cube_list_contents( cube_list_node *head)
{

	cube_list_node *temp;
	int contents = 0;

	if(head == NULL)
		return -1;

	for(temp = head -> next; temp != NULL; temp = temp -> next)
	{
		contents++;
	}

	return contents;
}

/*	cube_list_initialize func:
*
*	Creates a sentinel for new list, and returns
*	head pointer to it.
*	
*	Returns in succes the created head, else NULL 
*/
cube_list_node *cube_list_initialize()
{

	cube_list_node *head = malloc( sizeof(cube_list_node) );
	if ( head == NULL){
		printf("Error at creating list\n");
		return NULL;
	}

	head -> next = NULL;
	head -> prev = NULL;
	// "SENTINEL" is the attribute content for sentinel //
	head -> cube = strdup("SENTINEL");

	return head;
}

/*	cube_list_print func:
*	
*	Prints the contents of list, with the nodes
*	being seperated with enter.
*/
void cube_list_print( cube_list_node *head)
{
	
	cube_list_node *temp;
	
	// If head that doesn't excist, return //
	if(head == NULL)
	{
		return;
	}
	
	// If list is empty, nothing to print //
	if( head -> next == NULL)
	{
		return;
	}

	for(temp = head -> next; temp != NULL; temp = temp -> next)
	{
		printf("%s\n", temp->cube);

	}	
	putchar('\n');
}

/*	cube_list_getChar func:
*
*	Creates a string with the contents of list
*	with the nodes are seperated with ' '.
*	
*	Returns the created string in succes, else
*	NULL
*/
char *cube_list_getChar ( cube_list_node *head)
{

	char *out_string;

	cube_list_node *temp;
	int cube_length, contents;
	
	// If head that doesn't excist, return NULL //
	if( head == NULL)
	{
		return NULL;
	}
	
	// If list is empty, nothing to make in char //
	if( head -> next == NULL)
	{
		return NULL;
	}
	
	// Get num of contents of list and length of cube //
	cube_length = strlen( head -> next -> cube );
	contents = cube_list_contents(head);
	
	// Allocate  (cube_length + 1(for space) )*contents + 2(for {}) + 1(for \0) //
	out_string = ( char *) malloc ( sizeof(char) * cube_length * ( contents + 1 ) + 3);
	if(out_string == NULL)
		return NULL;
	
	//Build string //
	out_string[0] = '{';
	out_string[1] = ' ';
	out_string[2] = '\0';

	for(temp = head -> next; temp != NULL; temp = temp -> next)
	{
		strcat(out_string, temp -> cube);
		strcat(out_string, " ");

	}
	strcat(out_string, "}");

	return (out_string);


}

/*	cube_list_destroy func:
*
*	Destroyes the list with head as head
*/
void cube_list_destroy(cube_list_node *head)
{

	cube_list_node *temp;

	while ( head->next != NULL)
	{
		//Free content of node. //
		free ( head->next->cube );
		
		//Make pointer to next of head->next //
		temp = head->next;
		if(head->next != NULL)
		head->next = head->next->next;
		
		//Removes node //
		free(temp);
	}
	
	//Destroyes head //
	free(head->cube);
	free(head);
}

/*	cube_list_insert func:
*
*	Creates a new node in list with head as head, with 
*	content, the string new_cube and inserts it at the end..
*	
*	Returns 1 if inserted succesfully, else 0
*/
int cube_list_insert ( cube_list_node *head, char *new_cube)
{
	cube_list_node *tail;

	// Allocate memory for new node //
	cube_list_node *new_node = malloc( sizeof(cube_list_node) );
	if ( new_node == NULL){
		printf("Error at inserting to list\n");
		return 0;
	}
	
	for ( tail = head; tail -> next != NULL; tail = tail -> next);

	// Pointers from new node //
	new_node -> prev = tail;
	new_node -> next = tail -> next;

	tail -> next = new_node;
	
	// Make content of node //
	new_node -> cube = strdup(new_cube);

	return 1;
}

/*	cube_list_remove func:
*
*	Removes the node with the cube_to_remove string from list
*	with head as head.
*	
*	Returns 0 if removed succesfully, else 1
*/
int cube_list_remove ( cube_list_node *head, char *cube_to_remove)
{	

	cube_list_node *node_to_remove;

	//Find the node to remove
	node_to_remove = cube_list_search_node ( head, cube_to_remove );
	
	//If the node doesn't excist
	if ( node_to_remove == NULL )
		return 1;

	//Make pointers of neighboor nodes
	if ( node_to_remove -> prev != NULL)
		node_to_remove -> prev -> next = node_to_remove -> next;

	if ( node_to_remove -> next != NULL)
		node_to_remove -> next ->	prev = node_to_remove -> prev;

	//Free the allocated memory
	free ( node_to_remove );

	return 0;
}

/*	cube_list_search_node func:
*
*	Searches in the list with head as head, the node
*	with content the string cube
*	
*	Returns pointer to node, else NULL
*/
cube_list_node *cube_list_search_node ( cube_list_node *head , char *cube)
{

	cube_list_node *temp;
	
	// If head that doesn't excist, return NULL //
	if ( head == NULL)
	{
		return NULL;
	}
	
	// If list is empty, the node doesn't excist //
	if ( head -> next == NULL)
	{
		return NULL;
	}
	
	//Search in the list //
	for ( temp = head -> next; temp != NULL; temp = temp -> next)
	{
		if ( strcmp (temp -> cube, cube) == 0)
			return temp;

	}	
	return NULL;
}

/*	cube_funtions_union func:
*
*	Makes a list of two different lists of cubes
*
*	Returns a new list with sentinel if no problem occured, else NULL
*/
cube_list_node *cube_functions_union ( cube_list_node *head_1, cube_list_node *head_2 )
{

	cube_list_node *temp;
	cube_list_node *union_function;

	union_function = cube_list_initialize();
	if ( union_function == NULL )
		return NULL;

	if ( head_1 != NULL)
	{
		for ( temp = head_1 -> next; temp != NULL; temp = temp -> next)
		{
			cube_list_insert ( union_function, temp -> cube );
		}
	}

	if ( head_2 != NULL )
	{
		for ( temp = head_2 -> next; temp != NULL; temp = temp -> next)
		{
			cube_list_insert ( union_function, temp -> cube );
		}
	}

	return union_function;

}

cube_list_node *cube_functions_intersection ( cube_list_node *head_1, cube_list_node *head_2 )
{

	cube_list_node *temp;
	cube_list_node *intersected_function;

	if ( head_1 == NULL || head_2 == NULL )
		return NULL;

	intersected_function = cube_list_initialize();
	if ( intersected_function == NULL )
		return NULL;

	for ( temp = head_1 -> next; temp != NULL; temp = temp -> next)
	{
		if ( cube_list_search_node ( head_2, temp -> cube ) != NULL )
		{
			cube_list_insert ( intersected_function, temp -> cube );
		}
	}

	return intersected_function;

}

void table_cube_functions_intialize( table_cube_functions *table )
{

	table -> function = ( cube_list_node* *) malloc ( sizeof ( cube_list_node* ) );
	table -> function[0] = NULL;

	table -> size = 0;
}


void table_cube_functions_insert( table_cube_functions *table, cube_list_node *head )
{

	table -> function = realloc ( table -> function, sizeof ( cube_list_node* ) * ( table -> size + 1 ) );

	table -> size++;
	table -> function[ table -> size - 1] = cube_functions_union ( head, NULL );
	table -> function[ table -> size ]    = NULL;

}

void table_cube_functions_union ( table_cube_functions *table1, table_cube_functions *table2 )
{

	unsigned int temp_size;
	unsigned int i;

	table1 -> function = realloc ( table1 -> function, sizeof ( cube_list_node* ) * ( table1 -> size + table2 -> size ) );

	temp_size = table2 -> size + table1 -> size ;

	for ( i = 0; i < temp_size; i++ )
	{
		table1 -> function [ i + table1 -> size ] = table2 -> function [ i ];
	}

	table1 -> size = temp_size;

}

char *table_cube_functions_getChar ( table_cube_functions table )
{

	char *out_string;

	unsigned int i;
	int inter_strings_length = 0;
	
	// If no correct initialization, return NULL //
	if( table.function == NULL)
	{
		return NULL;
	}
	
	// If no lists in, nothing to make in char //
	if( table.function[0] == NULL)
	{
		return NULL;
	}

	for( i = 0; i < table.size; i++)
	{
		inter_strings_length += strlen ( cube_list_getChar ( table.function [i] ) );
	}

	// Allocate  (cube_length + 1(for space) )*contents + 2(for {}) + 1(for \0) //
	out_string = ( char *) malloc ( sizeof(char) * inter_strings_length + 3);
	if(out_string == NULL)
		return NULL;
	
	//Build string //
	out_string[0] = '(';
	out_string[1] = ' ';
	out_string[2] = '\0';

	for( i = 0; i < table.size; i++)
	{
		strcat(out_string, cube_list_getChar ( table.function[i]) );
		strcat(out_string, " ");

	}
	strcat(out_string, ")");

	return (out_string);
}

void table_cube_functions_destroy ( table_cube_functions *table )
{

	unsigned int i;

	for ( i = 0; i < table -> size; i++ )
	{
		cube_list_destroy ( table -> function [i] );
	}
}








