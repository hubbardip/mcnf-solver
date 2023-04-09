#include "mcnf.h"

int main(int argc, char **argv) {

  graph *g = read_dimacs("sample.min");
  /*graph *g = g_create();

  //generate n-node cycle:
  int n = 4;
  for(int i = 0; i < n; i++)
    g_add_node(g, 0);

  for(int i = 0; i < n - 1; i++) 
    g_add_arc(g, i, i+1, 1, 0, INFINITY);

  g_add_arc(g, n-1, 0, 1, 0, INFINITY);

  ((arc *) v_get(g->arcs, 0))->x = 1;*/

  remove_lower_bounds(g);
  //printf("LB: %d\n", check_lower_bounds(g));

  write_dot(g, "g.dot");

  add_interchange(g);

  ssp(g);

  if(check_interchange(g))
    printf("Problem infeasible; interchange vertex used\n");
  else {
    remove_interchange(g);
    printf("\n");
    if(check_comp_slackness(g))
      printf("Optimal solution found\n");
    else
      printf("No optimal solution\n");
    printf("Total cost = %g. Result:\n", total_cost(g));
    for(int i = 0; i < g->nodes->count; i++) {
      node *n = g_get_node(g, i);
      printf("Node %d: pi = %g\n", i, n->pi);
      for(int j = 0; j < n->out_arcs->count; j++) {
	arc *a = v_get(n->out_arcs, j);
	printf("  %d -> %d, flow = %g\n", a->src->idx, a->dst->idx, a->x);
      }
    }
  }
    
  g_free(g);

  return 0;
}
