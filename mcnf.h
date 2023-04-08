#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

typedef struct node {
  double supply;
  vector *out_arcs, *in_arcs;
  int idx;
  double pi, e;
} node;

typedef struct arc {
  double cost, lb, ub;
  node *src, *dst;
  int srci, dsti;
  int idx;
  double x;
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

/* Create a residual network with respect to a given flow */
graph *residual(graph *g);
