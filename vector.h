#include <stdlib.h>

typedef struct vector {
  void **items;
  int cap;
  int count;
} vector;

vector *v_create();
void v_init(vector *);
void v_resize(vector *, int);
void v_append(vector *, void *);
void *v_get(vector *, int);
void v_set(vector *, int, void *);
void v_remove(vector *, int);
void v_free(vector *);
void v_destroy(vector *);
