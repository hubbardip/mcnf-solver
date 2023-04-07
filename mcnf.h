#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

typedef struct node {
  double supply;
  vector *out_arcs, *in_arcs;
  int idx;
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
  int nnodes, narcs;
} graph;

graph *g_create();
/* Adds a node with supply "supply" */
void g_add_node(graph *g, double supply);

/* Adds an arc between node "start" and node "end" */
void g_add_arc(graph *g, int start, int end, double cost, double lb, double ub);

void g_free(graph *g);

/* Read a graph from a file in DIMACS format */
graph *read_dimacs(char *path);

/* Write a graph in the graphviz dot format */
void write_dot(graph *g, char *path);

/* Create a residual network with respect to a given flow */
graph *residual(graph *g);
