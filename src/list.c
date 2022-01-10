//
// list.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//
#include "list.h"
#include <assert.h>

int list_init(list_t *self) {
    if (!self)
        return -1;
    self->head = NULL;
    self->tail = NULL;
    self->match = NULL;
    self->len = 0;
    return 0;
}

list_node_t* list_rpush(list_t *self, list_node_t *node) {
    if (!self)
        return NULL;

    if (self->len) {
        node->prev = self->tail;
        node->next = NULL;
        self->tail->next = node;
        self->tail = node;
    } else {
        self->head = self->tail = node;
        node->prev = node->next = NULL;
    }

    ++self->len;
    return node;
}

list_node_t* list_rpop(list_t *self) {
    if (self->len == 0)
        return NULL;

    list_node_t *node = self->tail;

    if (--self->len) {
        (self->tail = node->prev)->next = NULL;
    } else {
        self->tail = self->head = NULL;
    }

    node->next = node->prev = NULL;
    return node;
}

list_node_t* list_lpop(list_t *self) {
    if (!self->len)
        return NULL;

    list_node_t *node = self->head;

    if (--self->len) {
        (self->head = node->next)->prev = NULL;
    } else {
        self->head = self->tail = NULL;
    }

    node->next = node->prev = NULL;
    return node;
}

list_node_t* list_lpush(list_t *self, list_node_t *node) {
    if (!node)
        return NULL;

    if (self->len) {
        node->next = self->head;
        node->prev = NULL;
        self->head->prev = node;
        self->head = node;
    } else {
        self->head = self->tail = node;
        node->prev = node->next = NULL;
    }

    ++self->len;
    return node;
}

/*
 * Return the node associated to val or NULL.
 */
list_node_t* list_find(list_t *self, list_node_obj_t *obj) {
    list_iterator_t it;
    list_node_t *node;

    list_iterator_create(&it, self, LIST_HEAD);

    while ((node = list_iterator_next(&it))) {
        if (self->match) {
            if (self->match(obj, node->obj)) {
                return node;
            }
        } else {
            if (obj == node->obj) {
                return node;
            }
        }
    }
    return NULL;
}

/*
 * Return the node at the given index or NULL.
 */

list_node_t* list_at(list_t *self, int index) {
    list_direction_t direction = LIST_HEAD;

    if (index < 0) {
        direction = LIST_TAIL;
        index = ~index;
    }

    if ((unsigned) index < self->len) {
        list_iterator_t it;
        list_iterator_create(&it, self, direction);
        list_node_t *node = list_iterator_next(&it);
        while (index--)
            node = list_iterator_next(&it);
        return node;
    }

    return NULL;
}

/*
 * Remove the given node from the list, freeing it and it's value.
 */

void list_remove(list_t *self, list_node_t *node) {
    node->prev ? (node->prev->next = node->next) : (self->head = node->next);

    node->next ? (node->next->prev = node->prev) : (self->tail = node->prev);

    --self->len;
}
