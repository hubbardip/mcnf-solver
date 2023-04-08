#include "vector.h"

vector *v_create() {
  vector *v = malloc(sizeof(vector));
  v_init(v);
  return v;
}

void v_init(vector *v) {
  v->cap = 16;
  v->count = 0;
  v->items = calloc(v->cap, sizeof(void *));
}

void v_resize(vector *v, int newcap) {
  void **items = realloc(v->items, sizeof(void *) * newcap);
  v->items = items;
  v->cap = newcap;
}

void v_append(vector *v, void *item) {
  if(v->count == v->cap)
    v_resize(v, 2*v->cap);
  v->items[v->count++] = item;
}

void *v_get(vector *v, int i) {
  if(i >= 0 && i < v->count)
    return v->items[i];
  return NULL;
}

void v_set(vector *v, int i, void *val) {
  if(i >= 0 && i < v->count)
    v->items[i] = val;
}

void v_remove(vector *v, int i) {
  if(i < 0 || i >= v->count) return;
  for(int j = i; j < v->count-1; j++)
    v->items[j] = v->items[j+1];
  v->count--;
}

void v_free(vector *v) {
  free(v->items);
}

void v_destroy(vector *v) {
  v_free(v);
  free(v);
}
