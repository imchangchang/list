//
// iterator.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

/*
 * Allocate a new list_iterator_t. NULL on failure.
 * Accepts a direction, which may be LIST_HEAD or LIST_TAIL.
 */

int list_iterator_create(list_iterator_t *self,
                         list_t *list,
                         list_direction_t direction) {
    list_node_t *node = (direction == LIST_HEAD) ? list->head : list->tail;
    return list_iterator_create_from_node(self, node, direction);
}

/*
 * Allocate a new list_iterator_t with the given start
 * node. NULL on failure.
 */

int list_iterator_create_from_node(list_iterator_t *self,
                                   list_node_t *node,
                                   list_direction_t direction) {
    if (self == NULL)
        return -1;
    self->next = node;
    self->direction = direction;
    return 0;
}

/*
 * Return the next list_node_t or NULL when no more
 * nodes remain in the list.
 */

list_node_t* list_iterator_next(list_iterator_t *self) {
    list_node_t *curr = self->next;
    if (curr) {
        self->next = self->direction == LIST_HEAD ? curr->next : curr->prev;
    }
    return curr;
}
