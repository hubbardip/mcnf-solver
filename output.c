#include "mcnf.h"

void write_dot(graph *g, char *path) {
  FILE *fp = fopen(path, "w");
  fprintf(fp, "digraph g {\n");
  
  for(int i = 0; i < g->nodes->count; i++) {
    node *n = v_get(g->nodes, i);
    fprintf(fp, "n%d [label=\"%d\"] ;\n", n->idx, n->idx);
  }

  for(int j = 0; j < g->arcs->count; j++) {
    arc *a = v_get(g->arcs, j);
    fprintf(fp, "n%d -> n%d [label=\"(%g, %g, %g)\"] ;\n", a->srci, a->dsti, a->cost, a->lb, a->ub);
  }

  //Legend:
  fprintf(fp, "subgraph cluster { label = \"Legend\"\ni [label=\"i\"] ;\nj [label=\"j\"] ;\n i -> j [label=<(c<SUB>ij</SUB>, l<SUB>ij</SUB>, u<SUB>ij</SUB>)>] ;}\n");

  fprintf(fp, "}\n");
  fclose(fp);
}
