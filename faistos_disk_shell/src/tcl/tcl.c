#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl8.6/tcl.h>

#include "tcl.h"

//lsCmd, implementation of ls command
int lsCmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]){

	char *cmd;
	char *arg1, *arg2;
	int arg1_length, arg2_length;

	// Check the number of arguments //
	if(argc > 3)
	{
		Tcl_SetResult( interp, "Wrong number of arguments", clientdata);
		return TCL_ERROR;
	}

	if(argc == 2)
	{
		arg1 = Tcl_GetStringFromObj(argv[1], &arg1_length);
		if (arg1 == NULL)
		{
			Tcl_SetResult( interp, "Error at Tcl_GetStringFromObj", clientdata);
			return TCL_ERROR;
		}

		cmd = (char *) malloc((arg1_length + 4) * sizeof(char));  //2 for ls 1 for whitespace and 1 for \0
		if (cmd == NULL)
		{
			printf("Problem in allocation of memory. Program exits...\n");
			exit(1);
		}

		sprintf(cmd, "ls %s", arg1);
	}
	else if(argc == 3)
	{
		arg1 = Tcl_GetStringFromObj(argv[1], &arg1_length);
		arg2 = Tcl_GetStringFromObj(argv[2], &arg2_length);

		if(arg1 == NULL || arg2 == NULL)
		{
			printf("Error at Tcl_GetStringFromObj\n");
			return TCL_ERROR;
		}

		cmd = (char *) malloc((arg1_length + arg2_length + 5) * sizeof(char));  //2 for ls 2 for whitespace and 1 for \0
		if (cmd == NULL)
		{
			printf("Problem in allocation of memory. Program exits...\n");
			exit(1);
		}

		sprintf(cmd, "ls %s %s", arg1, arg2);
	
	}
	else
	{
		cmd = strdup("ls");
		if (cmd == NULL){
			printf("Problem in allocation of memory. Program exits...\n");
			exit(1);
		}
	}

	system(cmd);
	free(cmd);

	return TCL_OK;

}

//lessCmd, implementation of less command
int lessCmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	char *cmd;
	char *arg;
	int length = 0;

	// Check the number of arguments //
	if (argc != 2)
	{
		printf("Wrong number of arguments!\n");
		return TCL_OK;
	}

	arg = Tcl_GetStringFromObj(argv[1], &length);
	if (arg == NULL)
	{
		printf("Error at Tcl_GetStringFromObj\n");
		return TCL_ERROR;
	}

	cmd = (char *) malloc((length + 6) * sizeof(char)); //4 for less, 1 for whitespace and 1 for \0
	if (cmd == NULL)
	{
		printf("Unable to Allocate Memory! Exiting...\n");
		exit(1);
	}

	sprintf(cmd, "less %s", arg);

	system(cmd);
	free(cmd);

	return TCL_OK;
}

/*	cube_intersect_2Cmd func:
*
*	Implementation of cube_intersect_2 command. Calls make_intersect with cubes
*	the strings given in arg[1] and argv[2] and creates TCL_Object string 
*	with the result.
*/
int cube_intersect_2Cmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	int cube1_length, cube2_length;
	char *argStr1, *argStr2;

	// Check the number of arguments //
	if(argc != 3){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	// Get first cube and check validation //
	if( ( argStr1 = Tcl_GetStringFromObj( argv[1], &cube1_length) ) == NULL )
	{
		return TCL_ERROR;
	}
	if ( not_valid_cube( argStr1) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Get second cube and check validation //
	if ( ( argStr2 = Tcl_GetStringFromObj( argv[2], &cube2_length) ) == NULL )
	{
		return TCL_ERROR;
	}
	if ( not_valid_cube( argStr2) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Check if cubes have same dimensions //
	if( cube1_length != cube2_length ){
		Tcl_SetResult( interp, "The 2 cubes have different length!", clientdata);
		return TCL_ERROR;
	}

	// Create intersect //
	char *new_cube = make_intersect (argStr1, argStr2);
	if( new_cube == NULL)
		return TCL_ERROR;

	// Set result //
	Tcl_SetResult( interp, new_cube, clientdata);
	return TCL_OK;
}

/*	supercube_2Cmd func:
*
*	Implementation of supercube_2 command. Calls make_supercube with cubes
*	the strings given in arg[1] and argv[2] and creates TCL_Object string 
*	with the result.
*/
int supercube_2Cmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{

	int cube1_length, cube2_length;
	char *argStr1, *argStr2;

	// Check the number of arguments //
	if(argc != 3){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	// Get first cube and check validation //
	if( ( argStr1 = Tcl_GetStringFromObj( argv[1], &cube1_length) ) == NULL )
	{
		return TCL_ERROR;
	}
	if ( not_valid_cube( argStr1) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Get second cube and check validation //
	if ( ( argStr2 = Tcl_GetStringFromObj( argv[2], &cube2_length) ) == NULL )
	{
		return TCL_ERROR;
	}
	if ( not_valid_cube( argStr2) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Check if cubes have same dimensions //
	if( cube1_length != cube2_length ){
		Tcl_SetResult( interp, "The 2 cubes have different length!", clientdata);
		return TCL_ERROR;
	}
	
	// Create supercube //
	char *new_cube = make_supercube(argStr1, argStr2);
	if( new_cube == NULL)
		return TCL_ERROR;
	
	// Set result //
	Tcl_SetResult( interp, new_cube, clientdata);
	return TCL_OK;
}

/*	distance_2Cmd func:
*
*	Implementation of distance_2 command. Calls make_intersect with cubes
*	the strings given in arg[1] and argv[2] and checks the double zeros in
*	the result. Creates TCL_Object string with number of distance.
*/
int distance_2Cmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	//Initialize distance at zero
	unsigned int distance = 0;
	char distance_in_string[MAX_DISTANCE_DIGIT];
	char *distance_in_string_dup;

	int cube1_length, cube2_length;
	char *argStr1, *argStr2;

	// Check the number of arguments //
	if(argc != 3){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	// Get first cube and check validation //
	if( ( argStr1 = Tcl_GetStringFromObj( argv[1], &cube1_length) ) == NULL )
	{
		return TCL_ERROR;
	}
	if ( not_valid_cube( argStr1) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Get second cube and check validation //
	if( ( argStr2 = Tcl_GetStringFromObj( argv[2], &cube2_length) ) == NULL )
	{
		return TCL_ERROR;
	}
	if ( not_valid_cube( argStr2) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Check if cubes have same dimensions //
	if( cube1_length != cube2_length ){
		Tcl_SetResult( interp, "The 2 cubes have different length!", clientdata);
		return TCL_ERROR;
	}

	// Make intersect between cubes //
	char *new_cube = make_intersect(argStr1, argStr2);

	if( new_cube == NULL)
		return TCL_ERROR;
	
	// Check the double zeros in intersection, which is the distance //
	distance = double_zeros(new_cube);	

	free ( new_cube);
	
	// Set result //
	sprintf(distance_in_string, "%d", distance);
	distance_in_string_dup = strdup ( distance_in_string);
	Tcl_SetResult( interp, distance_in_string_dup , clientdata);
	

	return TCL_OK;
}

/*	cube_cover_2Cmd func:
*
*	Implementation of cube_cover_2 command. Calls check_coverage with cubes
*	the strings given in arg[1] and argv[2] and creates TCL_Object string
*	with number of coverage.
*/
int cube_cover_2Cmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	char *argStr1, *argStr2;
	int cube1_length, cube2_length;

	int coverage;
	char coverage_in_string[MAX_COVERAGE_DIGIT];
	char *coverage_in_string_dup;

	// Check the number of arguments //
	if(argc != 3){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}
	
	// Get first cube and check validation //
	if( ( argStr1 = Tcl_GetStringFromObj( argv[1], &cube1_length) ) == NULL )
	{
		return -1;
	}
	if ( not_valid_cube( argStr1) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Get second cube and check validation //
	if( ( argStr2 = Tcl_GetStringFromObj( argv[2], &cube2_length) ) == NULL )
	{
		return -1;
	}
	if ( not_valid_cube( argStr2) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Check if cubes have same dimensions //
	if( cube1_length != cube2_length ){
		Tcl_SetResult( interp, "The 2 cubes have different length!", clientdata);
		return -1;
	}

	// Call check_coverage //
	coverage = check_coverage(argStr1, argStr2);
	
	// Set result //
	sprintf(coverage_in_string, "%d", coverage);
	coverage_in_string_dup = strdup ( coverage_in_string);
	Tcl_SetResult( interp, coverage_in_string_dup , clientdata);

	return TCL_OK;

}

/*	sharp_2Cmd func:
*
*	Implementation of sharp_2 command. Calls make_sharp_list with cubes
*	the strings given in arg[1] and argv[2].
*/
int sharp_2Cmd (ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]) 
{

	int cube1_length, cube2_length;
	char *argStr1, *argStr2;
	cube_list_node *sharp_list;

	// Check the number of arguments //
	if(argc != 3){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	// Get first cube and check validation //
	if( ( argStr1 = Tcl_GetStringFromObj( argv[1], &cube1_length) ) == NULL )
	{
		return TCL_ERROR;
	}
	if ( not_valid_cube( argStr1) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Get second cube and check validation //
	if( ( argStr2 = Tcl_GetStringFromObj( argv[2], &cube2_length) ) == NULL )
	{
		return TCL_ERROR;
	}
	if ( not_valid_cube( argStr2) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}

	// Check if cubes have same dimensions //
	if( cube1_length != cube2_length ){
		printf("The 2 cubes have different length!\n");
		return TCL_ERROR;
	}

	// Call make_sharp_list //
	sharp_list = make_sharp_list( argStr1, argStr2);

	// Set result //
	Tcl_SetResult( interp, cube_list_getChar ( sharp_list ), clientdata);

	return TCL_OK;

}

/*	sharpCmd func:
*
*	Implementation of sharp command. Calls sharp_cube_function with cube
*	the string given in arg[1] and function the list given argv[2].
*/
int sharpCmd (ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{

	unsigned int i;

	Tcl_Obj **function_argv;
	int function_argc;

	char *cube;
	char **function_cube;

	int cube_length;

	cube_list_node *result;
	
	// Check the number of arguments //
	if(argc != 3){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	// Get cube and check validation //
	cube = Tcl_GetStringFromObj( argv[1], &cube_length);
	if( cube == NULL )
	{
		return TCL_ERROR;
	}
	if ( not_valid_cube( cube) ){
		Tcl_SetResult( interp, "Check again input cube!", clientdata);
		return TCL_ERROR;
	}
	
	// Get function //
	Tcl_ListObjGetElements(interp, argv[2], &function_argc, &function_argv);
	function_cube = ( char* *) malloc ( sizeof(char*) * function_argc );
	if( function_cube == NULL)
	{
		return TCL_ERROR;
	}

	// Get cubes of function and check their validation //
	for ( i = 0; i < function_argc; i++)
	{
		function_cube[i] = Tcl_GetStringFromObj( function_argv[i], NULL);
		if(function_cube[i] == NULL)
		{
			return TCL_ERROR;
		}
		if ( not_valid_cube( function_cube[i]) )
		{
			Tcl_SetResult( interp, "Check again input cube in function!", clientdata);
			return TCL_ERROR;
		}
		if( strlen ( function_cube[i]) != cube_length )
		{
			Tcl_SetResult( interp, "Check length again!", clientdata);
			return TCL_ERROR;
		}

	}
	
	// Call sharp_cube_function //
	result = sharp_cube_function ( cube, function_cube, function_argc);
	
	free ( function_cube);

	// Set result //
	Tcl_SetResult( interp, cube_list_getChar ( result ), clientdata);

	return TCL_OK;

}

/*	OFF_fCmd func:
*
*	Implementation of OFF_f command. Calls sharp_cube_function with cube
*	the universe cube and function the given list in argv[1].
*/
int OFF_fCmd (ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{

	unsigned int i;

	Tcl_Obj **function_argv;
	int function_argc;
	int cube_length;

	char *universal_cube;
	char **function_cube;

	cube_list_node *result;
	
	// Check the number of arguments //
	if(argc != 2){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	// Initialize a table of strings for each cube of function //
	Tcl_ListObjGetElements(interp, argv[1], &function_argc, &function_argv);
	function_cube = ( char* *) malloc ( sizeof(char*) * function_argc );
	if( function_cube == NULL)
	{
		return TCL_ERROR;
	}
	
	// Put the cubes in the table if all are valid //
	for ( i = 0; i < function_argc; i++)
	{
		function_cube[i] = Tcl_GetStringFromObj( function_argv[i], NULL);
		if(function_cube[i] == NULL)
		{
			return TCL_ERROR;
		}
		if ( not_valid_cube( function_cube[i]) )
		{
			Tcl_SetResult( interp, "Check again input cube in function!", clientdata);
			return TCL_ERROR;
		}
	}
	
	// Take the cube length and check if all the cubes have the same (Have same dimensions) //
	cube_length = strlen(function_cube[0]);
	for( i = 0; i < function_argc; i++)
	{
		if( strlen ( function_cube[i]) != cube_length )
		{
			Tcl_SetResult( interp, "Check length again!", clientdata);
			return TCL_ERROR;
		}
	}
	
	// Make universal cube for the given dimensions //
	universal_cube = ( char *) malloc ( sizeof(char) * ( cube_length + 1) );
	if( universal_cube == NULL)
	{
		return TCL_ERROR;
	}

	for( i = 0; i < cube_length ; i++)
	{
		universal_cube[i] = '1';
	}
	universal_cube[i] = '\0';

	// Call sharp_cube_function //
	result = sharp_cube_function ( universal_cube, function_cube, function_argc);

	free ( function_cube);
	free ( universal_cube);

	// Set result //
	Tcl_SetResult( interp, cube_list_getChar ( result ), clientdata);
	return TCL_OK;

}

int read_graphCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] )
{

	char *file;
	int res;

	if(argc != 2){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	if( ( file = Tcl_GetStringFromObj( argv[1], NULL) ) == NULL )
	{	
		return TCL_ERROR;
	}

	res = create_graph_from_file ( file );
	if ( res == 2  )
	{
		Tcl_SetResult( interp, "Wrong input format file", clientdata);
	}
	else if ( res == -1 )
	{
		return TCL_ERROR;
	}

	return TCL_OK;
}

int write_graphCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] )
{

	char *file;
	int res;

	if(argc != 2){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	if( ( file = Tcl_GetStringFromObj( argv[1], NULL) ) == NULL )
	{	
		return TCL_ERROR;
	}

	res =  write_graph_to_file ( file );
	if (res == -1)
	{
		return TCL_ERROR;
	}
	else if (res == 1)
	{
		Tcl_SetResult( interp, "There is no graph!", clientdata);
	}

	return TCL_OK;
}

int graph_critical_pathCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] )
{

	char *max_dist_str;
	int max_dist;

	if(argc != 2){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	if( ( max_dist_str = Tcl_GetStringFromObj( argv[1], NULL) ) == NULL )
	{	
		return TCL_ERROR;
	}

	max_dist = atoi ( max_dist_str );

	Tcl_SetResult( interp, make_critical_path_and_slack ( max_dist ), clientdata);

	return TCL_OK;
}

int draw_graphCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] )
{

	char *file;
	int file_name_length;
	int res;

	char *cmd;

	if(argc != 2){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	if( ( file = Tcl_GetStringFromObj( argv[1], &file_name_length) ) == NULL )
	{	
		return TCL_ERROR;
	}

	res =  draw_graphiz ( file );
	if (res == -1)
	{
		return TCL_ERROR;
	}
	else if (res == 1)
	{
		Tcl_SetResult( interp, "There is no graph!", clientdata);
		return TCL_OK;
	}

	cmd = malloc ( sizeof ( char )* (file_name_length + 4 + 1 + 1));
	if ( cmd == NULL )
	{
		Tcl_SetResult( interp, "Error: Allocation of memory failed. Exit...", clientdata);
		exit(1);
	}

	strcpy( cmd, "dotty ");
	strcat( cmd, file);

	system ( cmd);
	free ( cmd);

	return TCL_OK;
}

int alg_divisionCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] )
{

	unsigned int i;

	Tcl_Obj **divisor_argv;
	int divisor_argc;

	Tcl_Obj **dividend_argv;
	int dividend_argc;

	char **divisor_str, **dividend_str;

	int cube_length;

	//cube_list_node *result;
	
	// Check the number of arguments //
	if(argc != 3){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}
	
	// Get function //
	Tcl_ListObjGetElements(interp, argv[1], &divisor_argc, &divisor_argv);
	divisor_str = ( char* *) malloc ( sizeof(char*) * divisor_argc );
	if( divisor_str == NULL)
	{
		return TCL_ERROR;
	}

	if ( divisor_argc == 0 )
	{
		Tcl_SetResult( interp, "No cubes for divisor given!", clientdata);
		return TCL_ERROR;
	}

	// Get cubes of function and check their validation //
	for ( i = 0; i < divisor_argc; i++)
	{
		divisor_str[i] = Tcl_GetStringFromObj( divisor_argv[i], NULL);
		if(divisor_str[i] == NULL)
		{
			return TCL_ERROR;
		}
		if ( not_valid_cube( divisor_str[i]) )
		{
			Tcl_SetResult( interp, "Check again input divisor!", clientdata);
			return TCL_ERROR;
		}

	}

	cube_length = strlen ( divisor_str[0] );
	for ( i = 0; i < divisor_argc; i++)
	{

		if( strlen ( divisor_str[i]) != cube_length )
		{
			Tcl_SetResult( interp, "Check length again!", clientdata);
			return TCL_ERROR;
		}

	}

	// Get function //
	Tcl_ListObjGetElements(interp, argv[2], &dividend_argc, &dividend_argv);
	dividend_str = ( char* *) malloc ( sizeof(char*) * dividend_argc );
	if( dividend_str == NULL)
	{
		return TCL_ERROR;
	}

	if ( dividend_argc == 0 )
	{
		Tcl_SetResult( interp, "No cubes for dividend given!", clientdata);
		return TCL_ERROR;
	}

	// Get cubes of function and check their validation //
	for ( i = 0; i < dividend_argc; i++)
	{
		dividend_str[i] = Tcl_GetStringFromObj( dividend_argv[i], NULL);
		if(dividend_str[i] == NULL)
		{
			return TCL_ERROR;
		}
		if ( not_valid_cube( dividend_str[i]) )
		{
			Tcl_SetResult( interp, "Check again input dividend!", clientdata);
			return TCL_ERROR;
		}
		if( strlen ( dividend_str[i]) != cube_length )
		{
			Tcl_SetResult( interp, "Check length again!", clientdata);
			return TCL_ERROR;
		}

	}

	division_algebric_fromStr ( divisor_str, dividend_str, divisor_argc, dividend_argc );


	return TCL_OK;
}

/*	OFF_fCmd func:
*
*	Implementation of OFF_f command. Calls sharp_cube_function with cube
*	the universe cube and function the given list in argv[1].
*/
int r_kernelsCmd (ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{

	unsigned int i;

	Tcl_Obj **function_argv;
	int function_argc;
	int cube_length;

	char **function_cube;
	
	// Check the number of arguments //
	if(argc != 2){
		Tcl_SetResult( interp, "Wrong number of arguments!", clientdata);
		return TCL_ERROR;
	}

	// Initialize a table of strings for each cube of function //
	Tcl_ListObjGetElements(interp, argv[1], &function_argc, &function_argv);
	function_cube = ( char* *) malloc ( sizeof(char*) * function_argc );
	if( function_cube == NULL)
	{
		return TCL_ERROR;
	}
	
	// Put the cubes in the table if all are valid //
	for ( i = 0; i < function_argc; i++)
	{
		function_cube[i] = Tcl_GetStringFromObj( function_argv[i], NULL);
		if(function_cube[i] == NULL)
		{
			return TCL_ERROR;
		}
		if ( not_valid_cube( function_cube[i]) )
		{
			Tcl_SetResult( interp, "Check again input cube in function!", clientdata);
			return TCL_ERROR;
		}
	}
	
	// Take the cube length and check if all the cubes have the same (Have same dimensions) //
	cube_length = strlen(function_cube[0]);
	for( i = 0; i < function_argc; i++)
	{
		if( strlen ( function_cube[i]) != cube_length )
		{
			Tcl_SetResult( interp, "Check length again!", clientdata);
			return TCL_ERROR;
		}
	}
	
	// Set result //
	Tcl_SetResult( interp, r_kernels_fromStr ( function_cube, function_argc ), clientdata);

	free ( function_cube);

	return TCL_OK;

}

/*	ExtendTcl func:
*
*	Includes the implementations of the tcl commands made in this project
*/
void ExtendTcl (Tcl_Interp *interp) {
   
    Tcl_CreateObjCommand(interp, "ls", lsCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "less", lessCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "cube_intersect_2", cube_intersect_2Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "supercube_2", supercube_2Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "distance_2", distance_2Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "cube_cover_2", cube_cover_2Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "sharp_2", sharp_2Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "sharp", sharpCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "OFF_f", OFF_fCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "read_graph", read_graphCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "write_graph", write_graphCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "graph_critical_path", graph_critical_pathCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "draw_graph", draw_graphCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "alg_division", alg_divisionCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "r_kernels", r_kernelsCmd, NULL, NULL);
}

