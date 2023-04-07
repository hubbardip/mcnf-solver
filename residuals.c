#include "mcnf.h"

graph *residual(graph *g) {
  if(!check_capacity(g)) {
    printf("Attempted to create residual network from invalid pseudoflow\n");
    return NULL;
  }

  if(!check_lower_bounds(g)) {
    printf("Attempted to create residual network from network with lower bounds\n");
    return NULL;
  }
  
  graph *res = g_create();
  for(int i = 0; i < g->nodes->count; i++)
    g_add_node(res, 0);
  
  for(int j = 0; j < g->arcs->count; j++) {
    arc *a = v_get(g->arcs, j);
    node *s = a->src;
    node *d = a->dst;
    
    if(a->ub - a->x != 0) { //draw forward arc
      g_add_arc(res, s->idx, d->idx, a->cost, 0, a->ub - a->x);
    }
    if(a->x != 0) { //draw reverse arc
      g_add_arc(res, d->idx, s->idx, -a->cost, 0, a->x);
    }
  }

  return res;
}
