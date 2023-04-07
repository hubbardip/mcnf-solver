#include "mcnf.h"

void remove_lower_bounds(graph *g) {
  for(int j = 0; j < g->arcs->count; j++) {
    arc *a = v_get(g->arcs, j);
    if(a->lb == 0) continue;
    a->src->supply -= a->lb;
    a->dst->supply += a->lb;
    a->ub -= a->lb;
    a->x -= a->lb;
    a->lb = 0;
  }
}

int check_lower_bounds(graph *g) {
  for(int j = 0; j < g->arcs->count; j++)
    if(((arc *) v_get(g->arcs, j))->lb != 0) return 0;
  return 1;
}
