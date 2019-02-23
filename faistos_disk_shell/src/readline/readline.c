#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "readline.h"


char *tcl_cmd_table[] = {
	//===========================TCL CMNDS===========================//
	"after",
	"append",
	"apply",
	"argc",
	"argv",
	"argv0",
	"array",
	"auto_execok",
	"auto_import",
	"auto_load",
	"auto_mkindex",
	"auto_path",
	"auto_qualify",
	"auto_reset",
	"bgerror",
	"binary",
	"break",
	"catch",
	"cd",
	"chan",
	"clock",
	"close",
	"concat",
	"continue",
	"coroutine",
	"dde",
	"dict",
	"encoding",
	"env",
	"eof",
	"error",
	"errorCode",
	"errorInfo",
	"eval",
	"exec",
	"exit",
	"expr",
	"fblocked",
	"fconfigure",
	"fcopy",
	"file",
	"fileevent",
	"filename",
	"flush",
	"for",
	"foreach",
	"format",
	"gets",
	"glob",
	"global",
	"history",
	"http",
	"if",
	"incr",
	"info",
	"interp",
	"join",
	"lappend",
	"lassign",
	"lindex",
	"linsert",
	"list",
	"llength",
	"lmap",
	"load",
	"lrange",
	"lrepeat",
	"lreplace",
	"lsearch",
	"lset",
	"lsort",
	"mathfunc",
	"mathop",
	"memory",
	"msgcat",
	"my",
	"namespace",
	"next",
	"nextto",
	"oo::class",
	"oo::copy",
	"oo::define",
	"oo::objdefine",
	"oo::object",
	"open",
	"package",
	"parray",
	"pid",
	"pkg::create",
	"pkg_mkIndex",
	"platform",
	"platform::shell",
	"proc",
	"puts",
	"pwd",
	"re_syntax",
	"read",
	"refchan",
	"regexp",
	"registry",
	"regsub",
	"rename",
	"return",
	"safe",
	"scan",
	"seek",
	"self",
	"set",
	"socket",
	"source",
	"split",
	"string",
	"subst",
	"switch",
	"tailcall",
	"tcl::prefix",
	"tcl_endOfWord",
	"tcl_findLibrary",
	"tcl_interactive",
	"tcl_library",
	"tcl_nonwordchars",
	"tcl_patchLevel",
	"tcl_pkgPath",
	"tcl_platform",
	"tcl_precision",
	"tcl_rcFileName",
	"tcl_startOfNextWord",
	"tcl_startOfPreviousWord",
	"tcl_traceCompile",
	"tcl_traceExec",
	"tcl_version",
	"tcl_wordBreakAfter",
	"tcl_wordBreakBefore",
	"tcl_wordchars",
	"tcltest",
	"tell",
	"throw",
	"time",
	"tm",
	"trace",
	"transchan",
	"try",
	"unknown",
	"unload",
	"unset",
	"update",
	"uplevel",
	"upvar",
	"variable",
	"vwait",
	"while",
	"yield",
	"yieldto",
	"zlib",
	
	//============================MY CMNDS===========================//
	"less",
	"ls",
	"cube_intersect_2",
	"supercube_2",
	"distance_2",
	"cube_cover_2",
	"sharp_2",
	"sharp",
	"OFF_f",
	"read_graph",
	"write_graph",
	"draw_graph",
	"graph_critical_path",
	"alg_division",
	"r_kernels",
	 NULL
};


char *input_cleaner ( char *dirty_input )
{

	char *whitespace;
	char *left_legal_str;

	char *clean_output;

	whitespace = dirty_input;

	while ( *whitespace == ' ' )
		whitespace++;
	left_legal_str = whitespace;

	while ( *whitespace != '\0')
		whitespace++;

	while ( *(whitespace - 1) == ' ')
		whitespace--;
	*whitespace = '\0';

	clean_output = strdup ( left_legal_str );

	free ( dirty_input );

	return clean_output;
}

/*	cursom_completer func:
*
*	Handler called when tab is pressed
*  	Returns table of strings to complete
*/
char **custom_completer(const char *text, int start, int end)
{
	char **matches = NULL;
	rl_completion_append_character = '\0';

	/* Call completer for tcl commands */
	if (start == 0)														
		return rl_completion_matches(text, character_name_generator);

	/*Return NULL value, so call default completer (for files) */
	return matches;										
}

/*	character_name_generator func:
*
*	Checks in consequence of input, the possible strings to match
*  	Returns a string each time
*/
char *character_name_generator(const char *text, int state)
{
    static int list_index, len;
    char *name;

    /*Check if is first string to put in table*/
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    /*Checks loop table to find correct string*/
    while ((name = tcl_cmd_table[list_index++])) {
        if (strncmp(name, text, len) == 0) {

            return strdup(name);
        }
    }

    return NULL;
}
