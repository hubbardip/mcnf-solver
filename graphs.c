#include "mcnf.h"

graph *g_create() {
  graph *g = malloc(sizeof(graph));
  g->nodes = v_create();
  g->arcs = v_create();
  g->nnodes = g->narcs = 0;

  return g;
}

void g_add_node(graph *g, double supply) {
  node *n = malloc(sizeof(node));
  n->supply = supply;
  n->out_arcs = v_create();
  n->in_arcs = v_create();
  n->idx = g->nnodes++;
  v_append(g->nodes, n);
}

void g_add_arc(graph *g, int start, int end, double cost, double lb, double ub) {
  if(start < 0 || start > g->nodes->count-1 || end < 0 || end > g->nodes->count-1) {
    fprintf(stderr, "Invalid index to add arc\n");
    return;
  }
  arc *a = malloc(sizeof(arc));
  a->cost = cost;
  a->lb = lb;
  a->ub = ub;
  
  node *s = v_get(g->nodes, start);
  node *d = v_get(g->nodes, end);
  a->src = s;
  a->dst = d;
  a->srci = s->idx;
  a->dsti = d->idx;

  a->idx = g->narcs++;

  a->x = 0;

  v_append(s->out_arcs, a);
  v_append(d->in_arcs, a);

  v_append(g->arcs, a);
}

void g_free(graph *g) {
  for(int i = 0; i < g->nodes->count; i++) {
    v_destroy(((node *) v_get(g->nodes, i))->out_arcs);
    v_destroy(((node *) v_get(g->nodes, i))->in_arcs);
    free(v_get(g->nodes, i));
  }
  for(int i = 0; i < g->arcs->count; i++) {
    free(v_get(g->arcs, i));
  }
  v_destroy(g->nodes);
  v_destroy(g->arcs);

  free(g);
}
