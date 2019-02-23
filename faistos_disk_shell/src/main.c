#include "main.h"

int main(int argc, char *argv[]){

	unsigned long i;

	graph_item_table_initialize();

	// ==================Readline Initialisation  ======================//

	//readline result
	char *text = NULL; 
	char *textexpansion;

	int expansionresult;

	char cmd [ MAX_CMD_INPUT ];

	//use rl filename_completion_function(), the default filename completer:
	rl_completion_entry_function = NULL; 
	rl_attempted_completion_function = custom_completer;
	rl_completion_append_character = '\0';

	//readline commands history list - NULL terminated
	HIST_ENTRY **the_history_list; 

	//initialise history functions
	using_history();

	// ====================TCL Initialisation  ========================//

	// arg[0] -> current executable
	Tcl_FindExecutable(argv[0]);

	// make interp for tcl. tcl_interp is defined in src/tcl/tcl.h library
	tcl_interp = Tcl_CreateInterp();
	Tcl_Init(tcl_interp);

	// make new commands
	ExtendTcl(tcl_interp);


	//===================Interface with User==========================//

	printf ( ANSI_COLOR_CYAN  "\n******************************************************"     ANSI_COLOR_RESET);
	printf (                  "\n=============== THE FAISTOS DISK SHELL ===============\n");
	printf("\n\n");
	printf(                   "\n                 ===================");
	printf(                   "\n                //                 \\\\");
	printf(                   "\n               //  "ANSI_COLOR_CYAN" __   _     __   "ANSI_COLOR_RESET"\\\\");
	printf(                   "\n              //   "ANSI_COLOR_CYAN"|    | \\   |    "ANSI_COLOR_RESET"  \\\\");
	printf(                   "\n             ||    "ANSI_COLOR_CYAN"|__  |  |  |__   "ANSI_COLOR_RESET"  ||");
	printf(                   "\n             ||    "ANSI_COLOR_CYAN"|    |  |     |  "ANSI_COLOR_RESET"  ||");
	printf(                   "\n             \\\\  "ANSI_COLOR_CYAN"  |    |_/    __|"ANSI_COLOR_RESET"   //");
	printf(                   "\n              \\\\                    //");
	printf(                   "\n               \\\\                  //");
	printf(                   "\n                 ====================");
	printf("\n\n");
	printf (                  "\n         Welcome to the Faistos Disk Shell");
	printf (                  "\n        Productor: Patsianotakis Charalampos");
	printf ( ANSI_COLOR_CYAN  "\n******************************************************\n\n" ANSI_COLOR_RESET);

	
	while(1){

		//Get text command
		text = readline (ANSI_COLOR_GREEN "FDS > " ANSI_COLOR_RESET);
		text = input_cleaner ( text );
		
		if ( text != NULL )
		{

			expansionresult = history_expand ( text, &textexpansion);
			if ( ( expansionresult == 0 ) || ( expansionresult == 2 ) )
			{
				add_history ( text );
				strcpy ( cmd, text );
			}
			else
			{
				add_history ( textexpansion );
				strcpy ( cmd, textexpansion );
			}

			free ( textexpansion );
			free ( text );
		}

		//Handle QUIT
		if(strcmp(cmd, "quit") == 0)
		{
			return EXIT_SUCCESS;
		}

		//Handle HISTORY
		else if (strcmp(cmd, "history") == 0)
		{

			the_history_list = history_list(); //get history list
			if(the_history_list != NULL){

				i = 0;
				while( *(the_history_list + i) != NULL ) //history list - NULL terminated
				{
					printf("%lu: %s\n", (i + history_base), ( *(the_history_list + i))->line );
					i++;
				}
			}

		}

		//Execute a TCL command
		else
		{
			printf("\n");
			if( Tcl_Eval(tcl_interp, cmd) == TCL_ERROR){
				
				printf("Check your argument again!\n");
			}

			printf("%s\n\n", Tcl_GetStringResult(tcl_interp));
		}

	}

	return 0;
}