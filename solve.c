#include "mcnf.h"

double reduced_cost(arc *a) {
  return a->cost - a->src->pi + a->dst->pi;
}

/* dijkstra's algorithm - probably not efficient implementation */
vector *shortest_path(graph *g, int s, int t) {
  vector *Q = v_create(); //vector of nodes
  for(int i = 0; i < g->nodes->count; i++) {
    node *n = g_get_node(g, i);
    n->d = INFINITY;
    n->prev = NULL;
    v_append(Q, n);
    n->marked = 0; //equals 0 iff n is in Q
  }
  g_get_node(g, s)->d = 0;

  while(Q->count > 0) {
    double min_dist = INFINITY;
    int closest_i = -1;
    for(int i = 0; i < Q->count; i++) {
      node *n = v_get(Q, i);
      if(n->d < min_dist) {
	min_dist = n->d;
	closest_i = i;
      }
    }
    node *u = v_get(Q, closest_i);
    v_remove(Q, closest_i);
    u->marked = 1;
    for(int i = 0; i < u->out_arcs->count; i++) {
      arc *a = v_get(u->out_arcs, i);
      if(a->dst->marked) continue;
      double alt = u->d + a->cost;
      if(alt < a->dst->d) {
	a->dst->d = alt;
	a->dst->prev = a;
      }
    }

    //stop if we have marked t
    if(u->idx == t) break;
  }
  v_destroy(Q);

  //Construct the path
  vector *path_rev = v_create(); //vector of arcs in the path, starting from t
  node *curr = g_get_node(g, t);
  while(curr->prev != NULL) {
    v_append(path_rev, curr->prev);
    curr = curr->prev->src;
  }

  vector *path = v_create();
  for(int j = path_rev->count-1; j >= 0; j--)
    v_append(path, v_get(path_rev, j));

  v_destroy(path_rev);
  return path;
}
