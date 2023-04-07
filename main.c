#include "mcnf.h"

int main(int argc, char **argv) {
  graph *g = g_create();

  //generate n-node cycle:
  int n = 10000;
  for(int i = 0; i < n; i++)
    g_add_node(g, 0);

  for(int i = 0; i < n - 1; i++) 
    g_add_arc(g, i, i+1, 1, 0, INFINITY);

  g_add_arc(g, n-1, 0, 1, 0, INFINITY);

  g_free(g);

  return 0;
}
