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
      double alt = u->d + reduced_cost(a);
      if(alt < a->dst->d) {
	a->dst->d = alt;
	a->dst->prev = a;
      }
    }

    //stop if we have marked t
    //if(u->idx == t) break;
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

void ssp(graph *g) {
  vector *X = v_create(); //excess nodes
  vector *D = v_create(); //deficit nodes
  for(int i = 0; i < g->nodes->count; i++) {
    node *n = g_get_node(g, i);
    n->pi = 0;
    n->e = n->supply;
    if(n->e > 0) {
      v_append(X, n);
      printf("Added node %d to X\n", i);
    }
    else if(n->e < 0)
      v_append(D, n);
  }

  for(int j = 0; j < g->arcs->count; j++) {
    g_get_arc(g, j)->x = 0;
  }

  while(X->count > 0) {
    node *s = v_get(X, X->count-1);
    node *t = v_get(D, D->count-1);
    
    graph *res = residual(g);
    vector *p = shortest_path(res, s->idx, t->idx);

    double delta = (s->e < -t->e) ? s->e : -t->e;

    for(int j = 0; j < p->count; j++) {
      arc *a = v_get(p, j);
      if(a->ub < delta)
	delta = a->ub;
      //a->src->pi -= a->src->d;
      //a->src->orig->pi -= a->src->d;
      printf("res pi = %g, orig pi = %g\n", a->src->pi, a->src->orig->pi);
    }
    //t->pi -= g_get_node(res, t->idx)->d;
    //printf("Distance to node 1 = %g\n", g_get_node(res, 1)->d);
    printf("Reduced cost on arc 1 -> 2 = %g\n", reduced_cost(g_get_arc(res, 0)));
    //update pi:
    for(int i = 0; i < res->nodes->count; i++) {
      node *nres = g_get_node(res, i);
      node *norig = g_get_node(g, i);
      norig->pi -= nres->d;
    }
    for(int j = 0; j < p->count; j++) { //send delta flow along path
      arc *a = v_get(p, j);
      arc *o = a->orig;
      if(a->reverse) {
	o->x -= delta;
      } else {
	o->x += delta;
      }
      /*printf("Adding %g flow to arc %d = (%d, %d)\n", delta, j, a->src->idx, a->dst->idx);
	((arc *) v_get(p, j))->x += delta;*/
    }
    
    /*printf("\nStep. Result: \n");
    for(int i = 0; i < g->nodes->count; i++) {
      node *n = g_get_node(g, i);
      printf("Node %d: pi = %g\n", i, n->pi);
      for(int j = 0; j < n->out_arcs->count; j++) {
	arc *a = v_get(n->out_arcs, j);
	printf("  %d -> %d, flow = %g\n", a->src->idx, a->dst->idx, a->x);
      }
      }*/

    //update e, X, D (could be done more efficiently)
    g_free(res);
    v_destroy(p);
    v_destroy(X);
    v_destroy(D);
    X = v_create();
    D = v_create();
    for(int i = 0; i < g->nodes->count; i++) {
      node *n = g_get_node(g, i);
      n->e = n->supply;
      for(int j = 0; j < n->out_arcs->count; j++)
	n->e -= ((arc *) v_get(n->out_arcs, j))->x;
      for(int j = 0; j < n->in_arcs->count; j++)
	n->e += ((arc *) v_get(n->in_arcs, j))->x;
      
      if(n->e > 0)
	v_append(X, n);
      else if(n->e < 0)
	v_append(D, n);
    }
    /*if(s->e == 0)
      v_remove(X, X->count-1);
    if(t->e == 0)
    v_remove(D, D->count-1);*/
  }
  v_destroy(X);
  v_destroy(D);
}

double total_cost(graph *g) {
  double cost = 0;
  for(int j = 0; j < g->arcs->count; j++) {
    arc *a = g_get_arc(g, j);
    cost += a->x*a->cost;
  }
  return cost;
}

int check_comp_slackness(graph *g) {
  if(!check_flow(g)) return 0; //infeasible flow
  graph *res = residual(g);
  for(int j = 0; j < res->arcs->count; j++) {
    if(reduced_cost(g_get_arc(res, j)) < 0)
      return 0;
  }
  g_free(res);
  return 1;
}
