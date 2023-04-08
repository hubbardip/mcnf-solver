#include "mcnf.h"

int main(int argc, char **argv) {

  //graph *g = read_dimacs("sample.min");
  graph *g = g_create();

  //generate n-node cycle:
  int n = 4;
  for(int i = 0; i < n; i++)
    g_add_node(g, 0);

  for(int i = 0; i < n - 1; i++) 
    g_add_arc(g, i, i+1, 1, 0, INFINITY);

  g_add_arc(g, n-1, 0, 1, 0, INFINITY);

  ((arc *) v_get(g->arcs, 0))->x = 1;

  write_dot(g, "g.dot");

  add_interchange(g);
  write_dot(g, "inter.dot");

  remove_interchange(g);
  write_dot(g, "no_inter.dot");
  
  g_free(g);

  return 0;
}
