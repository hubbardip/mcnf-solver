#include "mcnf.h"

graph *g_create() {
  graph *g = malloc(sizeof(graph));
  g->nodes = v_create();
  g->arcs = v_create();
  g->node_idx = g->arc_idx = 0;
  g->interchange_arc = -1;

  return g;
}

int g_add_node(graph *g, double supply) {
  node *n = malloc(sizeof(node));
  n->supply = supply;
  n->out_arcs = v_create();
  n->in_arcs = v_create();
  n->idx = g->node_idx++;
  v_append(g->nodes, n);
  return n->idx;
}

int g_add_arc(graph *g, int start, int end, double cost, double lb, double ub) {
  if(start < 0 || start > g->nodes->count-1 || end < 0 || end > g->nodes->count-1) {
    fprintf(stderr, "Invalid index to add arc\n");
    return -1;
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

  a->idx = g->arc_idx++;

  a->x = 0;

  v_append(s->out_arcs, a);
  v_append(d->in_arcs, a);

  v_append(g->arcs, a);
  return a->idx;
}

void g_remove_arc(graph *g, int idx) {
  arc *a = v_get(g->arcs, idx);
  for(int j = 0; j < a->src->out_arcs->count; j++) {
    if(v_get(a->src->out_arcs, j) == a) {
      v_remove(a->src->out_arcs, j);
      break;
    }
  }
  for(int j = 0; j < a->dst->in_arcs->count; j++) {
    if(v_get(a->dst->in_arcs, j) == a) {
      v_remove(a->dst->in_arcs, j);
      break;
    }
  }
  free(a);

  //Remove arc, adjusting the indices of all later-occuring arcs
  for(int j = idx; j < g->arcs->count-1; j++) {
    g->arcs->items[j] = g->arcs->items[j+1];
    ((arc *) g->arcs->items[j+1])->idx--;
  }
  g->arcs->count--;
}

void g_remove_node(graph *g, int idx) {
  node *n = v_get(g->nodes, idx);
  while(n->out_arcs->count > 0) {
    //remove the last arc in the vector until it is empty
    g_remove_arc(g, ((arc *) v_get(n->out_arcs, n->out_arcs->count-1))->idx);
  }

  while(n->in_arcs->count > 0) {
    g_remove_arc(g, ((arc *) v_get(n->in_arcs, n->in_arcs->count-1))->idx);
  }
  
  v_destroy(n->out_arcs);
  v_destroy(n->in_arcs);
  free(n);
  //Remove node, adjusting the indices of all later-occuring nodes
  for(int j = idx; j < g->nodes->count-1; j++) {
    g->nodes->items[j] = g->nodes->items[j+1];
    ((node *) g->nodes->items[j+1])->idx--;
  }
  g->nodes->count--;
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
