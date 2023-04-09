#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

typedef struct node node;
typedef struct arc arc;

typedef struct node {
  double supply;
  vector *out_arcs, *in_arcs;
  int idx;
  double pi, e, d; //node potential, excess, distance along path
  arc *prev; //arc connecting to previous node in path
  int marked; //1 if already marked by dijkstra's
  node *orig; //for resnets, pointer to the node in the original
} node;

typedef struct arc {
  double cost, lb, ub;
  node *src, *dst;
  int srci, dsti;
  int idx;
  double x;
  arc *orig; //for resnets, pointer to the arc in the original
  int reverse; //equals 1 if the arc is the reverse of the original
} arc;

typedef struct graph {
  vector *nodes, *arcs;
  int node_idx, arc_idx; //indices of next node and arc to be added
  int interchange_arc; //index of interchange arc (a, b)
} graph;

graph *g_create();
/* Adds a node with supply "supply"
 * Returns the index of the new node */
int g_add_node(graph *g, double supply);

/* Adds an arc between node "start" and node "end"
 * Returns the index of the new arc */
int g_add_arc(graph *g, int start, int end, double cost, double lb, double ub);

/* Remove an arc */
void g_remove_arc(graph *g, int idx);

/* Remove a node and all incident arcs */
void g_remove_node(graph *g, int idx);

node *g_get_node(graph *g, int idx);

arc *g_get_arc(graph *g, int idx);

/* Destroy graph */
void g_free(graph *g);

/* Read a graph from a file in DIMACS format */
graph *read_dimacs(char *path);

/* Write a graph in the graphviz dot format */
void write_dot(graph *g, char *path);

/* Returns 1 iff flow on graph is feasible */
int check_flow(graph *g);

/* Returns 1 iff flow on graph satisfies capacity constraints */
int check_capacity(graph *g);

/* Returns 1 iff flow on graph satisfies mass-balance constraints */
int check_mass_balance(graph *g);

/* Transform network to set all lower bounds to 0 */
void remove_lower_bounds(graph *g);

/* Returns 1 iff all lower bounds are 0 */
int check_lower_bounds(graph *g);

/* Adds an interchange arc to ensure infinite-cost paths between all pairs of nodes */
void add_interchange(graph *g);

/* Removes the interchange nodes and arcs */
void remove_interchange(graph *g);

/* Returns 1 if interchange arc was used; i.e. problem infeasible */
int check_interchange(graph *g);

/* Create a residual network with respect to a given flow */
graph *residual(graph *g);

/* Send delta units of flow on arc a in residual network,
 * adjusting arcs as needed */
void res_send_flow(graph *res, arc *a, double delta);

/* Calculate shortest path s -> t wrt reduced costs
 * Returns a vector of arcs in the path */
vector *shortest_path(graph *g, int s, int t);

/* Run SSP to solve MCNF, storing results in pi and x */
void ssp(graph *g);

/* Return total cost of flow on g */
double total_cost(graph *g);

/* Return 1 if solution is optimal according to CS */
int check_comp_slackness(graph *g);
