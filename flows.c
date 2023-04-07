#include "mcnf.h"

int check_flow(graph *g) {
  return check_capacity(g) && check_mass_balance(g);
}

int check_capacity(graph *g) {
  for(int j = 0; j < g->arcs->count; j++) {
    arc *a = v_get(g->arcs, j);
    if(a->x > a->ub || a->x < a->lb)
      return 0;
  }
  return 1;
}

int check_mass_balance(graph *g) {
  for(int i = 0; i < g->nodes->count; i++) {
    node *n = v_get(g->nodes, i);
    double e = n->supply;
    for(int j = 0; j < n->in_arcs->count; j++)
      e += ((arc *) v_get(n->in_arcs, j))->x;
    for(int k = 0; k < n->out_arcs->count; k++)
      e -= ((arc *) v_get(n->out_arcs, k))->x;
    if(e != 0)
      return 0;
  }
  return 1;
}
