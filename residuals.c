#include "mcnf.h"

graph *residual(graph *g) {
  if(!check_lower_bounds(g)) {
    printf("Attempted to create residual network from network with lower bounds\n");
    return NULL;
  }
    
  if(!check_capacity(g)) {
    printf("Attempted to create residual network from invalid pseudoflow\n");
    return NULL;
  }
  
  graph *res = g_create();
  for(int i = 0; i < g->nodes->count; i++) {
    int k = g_add_node(res, 0);
    g_get_node(res, k)->orig = g_get_node(g, i);
    g_get_node(res, k)->pi = g_get_node(g, i)->pi;
  }
  
  for(int j = 0; j < g->arcs->count; j++) {
    arc *a = v_get(g->arcs, j);
    node *s = a->src;
    node *d = a->dst;
    
    if(a->ub - a->x != 0) { //draw forward arc
      int k = g_add_arc(res, s->idx, d->idx, a->cost, 0, a->ub - a->x);
      g_get_arc(res, k)->orig = a;
      g_get_arc(res, k)->reverse = 0;
    }
    if(a->x != 0) { //draw reverse arc
      int k = g_add_arc(res, d->idx, s->idx, -a->cost, 0, a->x);
      g_get_arc(res, k)->orig = a;
      g_get_arc(res, k)->reverse = 1;
    }
  }

  return res;
}

/*void res_send_flow(graph *res, arc *a, double delta) {
  //find reverse arc, if it exists
  node *s = a->src;
  node *d = a->dst;
  arc *r = NULL;
  for(int j = 0; j < s->out_arcs->count; j++) {
    arc *c = v_get(s->out_arcs, j);
    if(c->dst == d) {
      r = c;
      break;
    }
  }
  if(r == NULL) {
    //no reverse arc exists, so send flow on a and add a reverse arc
    a->x += delta;
    g_add_arc(res, d->idx, s->idx, -a->cost, 0, a->x);
  } else {
    //remove flow from reverse arc first
    if(delta >= r->x) {
      //new flow cancels with reverse arc, and there will be excess
      delta -= r->x;
      r->x = 0;
    } else {
      r->x -= delta;
    }
    a->x += delta;
    r->ub += delta;
  }
}*/
