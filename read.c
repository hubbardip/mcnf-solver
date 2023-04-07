#include "mcnf.h"

void parse_line(graph *g, char *line, size_t len) {
  char prob[10];
  int nodes, arcs, id, src, dst;
  double supply, low, cap, cost;
  switch(line[0]) {
  case 'c':
    break;
  case 'p':
    sscanf(line, "p %s %d %d\n", prob, &nodes, &arcs);
    for(int i = 0; i < nodes; i++)
      g_add_node(g, 0);
    break;
  case 'n':
    sscanf(line, "n %d %lf\n", &id, &supply);
    ((node *)v_get(g->nodes, id-1))->supply = supply;
    break;
  case 'a':
    sscanf(line, "a %d %d %lf %lf %lf\n", &src, &dst, &low, &cap,  &cost);
    g_add_arc(g, src-1, dst-1, cost, low, cap);
    break;
  default:
    printf("Invalid line %s\n", line);
  }
}

graph *read_dimacs(char *path) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  
  if(!(fp = fopen(path, "r"))) {
    printf("Error reading file %s\n", path);
    return NULL;
  }

  graph *g = g_create();

  while((read = getline(&line, &len, fp)) != -1)
    parse_line(g, line, len);

  free(line);
  fclose(fp);
  
  return g;
}
