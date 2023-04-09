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

void add_interchange(graph *g) {
  if(g->interchange_arc != -1) return; //interchange already exists

  //add an arc (a, b) of infinite capacity and impractically high cost
  //then add arcs (u, a) and (b, u) for all other nodes u

  double C = 0;
  for(int j = 0; j < g->arcs->count; j++)
    C += ((arc *) v_get(g->arcs, j))->cost;

  
  int ai, bi;
  ai = g_add_node(g, 0);
  bi = g_add_node(g, 0);

  g->interchange_arc = g_add_arc(g, ai, bi, C+1, 0, INFINITY);
  if(g->interchange_arc == -1)
    printf("Error adding interchange arc (a, b)\n");
  for(int i = 0; i < ai; i++) {
    g_add_arc(g, i, ai, 0, 0, INFINITY);
    g_add_arc(g, bi, i, 0, 0, INFINITY);
  }
}

void remove_interchange(graph *g) {
  if(g->interchange_arc == -1) return; //interchange DNE

  arc *a = v_get(g->arcs, g->interchange_arc);
  int src_i = a->src->idx;
  int dst_i = a->dst->idx; //needed, since a is destroyed in the first remove_node call
  g_remove_node(g, src_i);
  g_remove_node(g, (dst_i > src_i) ? dst_i - 1 : dst_i); //index will have been reduced by 1
}

int check_interchange(graph *g) {
  if(g->interchange_arc == -1) return 0; //interchange DNE
  arc *a = v_get(g->arcs, g->interchange_arc);
  return a->x != 0;
}
