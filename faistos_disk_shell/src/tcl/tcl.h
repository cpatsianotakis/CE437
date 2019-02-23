#ifndef FDS_TCL_L
#define FDS_TCL_L

#include "../main.h"

#define MAX_DISTANCE_DIGIT 12
#define MAX_COVERAGE_DIGIT 12

Tcl_Interp *tcl_interp;

// TCL Functions
int lsCmd(ClientData dummy, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int lessCmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int supercube_2Cmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int cube_intersect_2Cmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int distance_2Cmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int cube_cover_2Cmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int sharp_2Cmd(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int sharpCmd (ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int OFF_fCmd (ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int OFF_fCmd (ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int OFF_fCmd (ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int read_graphCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] );
int write_graphCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] );
int graph_critical_pathCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] );
int draw_graphCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] );
int r_kernelsCmd (ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int alg_divisionCmd ( ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[] );

void ExtendTcl (Tcl_Interp *interp);

#endif
