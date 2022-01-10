
//
// list.h
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#ifndef __CLIBS_LIST_H__
#define __CLIBS_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/*
 * list_t iterator direction.
 */

typedef enum {
    LIST_HEAD
  , LIST_TAIL
} list_direction_t;

typedef void list_node_obj_t;
typedef int (*list_node_obj_match_t)(list_node_obj_t*, list_node_obj_t*);

/*
 * list_t node struct.
 */

typedef struct list_node {
  struct list_node *prev;
  struct list_node *next;
  list_node_obj_t *obj;
} list_node_t;

/*
 * list_t struct.
 */

typedef struct {
  list_node_t *head;
  list_node_t *tail;
  unsigned int len;
  int (*match)(list_node_obj_t *a, list_node_obj_t *b);
} list_t;

/*
 * list_t iterator struct.
 */

typedef struct {
  list_node_t *next;
  list_direction_t direction;
} list_iterator_t;

// Node prototypes.
int list_node_create(list_node_t *self, list_node_obj_t *obj);

// list_t prototypes.
int list_init(list_t *self);
list_node_t* list_rpush(list_t *self, list_node_t *node);
list_node_t* list_rpop(list_t *self);
list_node_t* list_lpop(list_t *self);
list_node_t* list_lpush(list_t *self, list_node_t *node);
list_node_t* list_find(list_t *self, list_node_obj_t *obj);
list_node_t* list_at(list_t *self, int index);
void list_remove(list_t *self, list_node_t *node);

// list_t iterator prototypes.
int list_iterator_create(list_iterator_t *self, list_t *list, list_direction_t direction);
int list_iterator_create_from_node(list_iterator_t *self, list_node_t *node, list_direction_t direction);
list_node_t* list_iterator_next(list_iterator_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __CLIBS_LIST_H__ */
