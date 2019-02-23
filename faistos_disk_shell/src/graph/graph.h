#ifndef FDS_GRAPH_L
#define FDS_GRAPH_L

#include "../main.h"

struct graph_node_dist_struct
{
	struct graph_node_dist_struct *next;
	
	int index;
	int distance;
};
typedef struct graph_node_dist_struct graph_node_dist;

struct graph_node_struct
{
	struct graph_node_struct *next;

	int index;
	char *vertic_name;
	graph_node_dist *head_neighboors;
};
typedef struct graph_node_struct graph_item;

graph_item *GRAPH_TABLE;
int GRAPH_SIZE;

void graph_item_table_initialize ();
void graph_item_table_destroy();
void graph_item_table_printvertic();
int graph_item_table_getsize();
int graph_item_table_add_vertic ( char *vertic_name );
int graph_item_table_getindex ( char *vertic_name);
int graph_node_dist_add ( char *source_vertic_name, int drain_vertic_index, int distance);
int graph_index_deque ( int *table, int *current_size);
void graph_index_enque ( int *table, int *current_size, int new_value);

int decode_line ( char *line_str, int line );
int create_graph_from_file ( char *file_path );
int node_source_to_file ( graph_item item, int fd);
int write_graph_to_file ( char *file_name );
void set_inputs (int table[]);
void set_outputs (int table[]);
void make_distances ( int dist_table [] );
int longest_path (int longest_path_table [], int dist_table [] );
void get_slack ( int max_legal_dist, int slack [], int dist_table [] );
char *make_critical_path_and_slack ( int max_dist );
int draw_graphiz ( char *file_name );

#endif