//
// node.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

int list_node_create(list_node_t *self, list_node_obj_t *obj) {
    if (self == NULL || obj == NULL)
        return -1;
    self->prev = NULL;
    self->next = NULL;
    self->obj= obj;
    return 0;
}
