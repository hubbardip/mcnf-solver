#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

typedef struct node {
  double supply;
  vector *out_arcs, *in_arcs;
} node;

typedef struct arc {
  double cost, lb, ub;
  node *start, *end;
} arc;

typedef struct graph {
  vector *nodes, *arcs;
} graph;

graph *g_create();
/* Adds a node with supply "supply" */
void g_add_node(graph *g, double supply);

/* Adds an arc between node "start" and node "end" */
void g_add_arc(graph *g, int start, int end, double cost, double lb, double ub);

void g_free(graph *g);
