#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "src/list.h"

// Helpers

#define test(fn) \
  puts("... \x1b[33m" # fn "\x1b[0m"); \
  test_##fn();

static int freeProxyCalls = 0;

void freeProxy(void *val) {
    ++freeProxyCalls;
    free(val);
}

typedef struct {
    char *name;
} User;

static int User_equal(User *a, User *b) {
    return 0 == strcmp(a->name, b->name);
}

// Tests

static void test_list_node_create() {
    char *obj = "some value";
    list_node_t node;
    assert(list_node_create(&node, obj) == 0);
    assert(node.obj == obj);
}

static void test_list_rpush() {
    // Setup
    list_t _list;
    list_init(&_list);

    list_node_t _a;
    list_node_create(&_a, "a");
    list_node_t _b;
    list_node_create(&_b, "b");
    list_node_t _c;
    list_node_create(&_c, "c");

    list_t *list = &_list;
    list_node_t *a = &_a;
    list_node_t *b = &_b;
    list_node_t *c = &_c;

    // a b c
    list_rpush(list, a);
    list_rpush(list, b);
    list_rpush(list, c);

    // Assertions
    assert(a == list->head);
    assert(c == list->tail);
    assert(3 == list->len);
    assert(b == a->next);
    assert(NULL == a->prev);
    assert(c == b->next);
    assert(a == b->prev);
    assert(NULL == c->next);
    assert(b == c->prev);

}

static void test_list_lpush() {
    // Setup
    list_t _list;
    list_init(&_list);

    list_node_t _a;
    list_node_create(&_a, "a");
    list_node_t _b;
    list_node_create(&_b, "b");
    list_node_t _c;
    list_node_create(&_c, "c");

    list_t *list = &_list;
    list_node_t *a = &_a;
    list_node_t *b = &_b;
    list_node_t *c = &_c;
    // c b a
    list_rpush(list, a);
    list_lpush(list, b);
    list_lpush(list, c);

    // Assertions
    assert(c == list->head);
    assert(a == list->tail);
    assert(3 == list->len);
    assert(NULL == a->next);
    assert(b == a->prev);
    assert(a == b->next);
    assert(c == b->prev);
    assert(b == c->next);
    assert(NULL == c->prev);
}

static void test_list_at() {
    // Setup
    list_t _list;
    list_init(&_list);

    list_node_t _a;
    list_node_create(&_a, "a");
    list_node_t _b;
    list_node_create(&_b, "b");
    list_node_t _c;
    list_node_create(&_c, "c");

    list_t *list = &_list;
    list_node_t *a = &_a;
    list_node_t *b = &_b;
    list_node_t *c = &_c;

    // a b c
    list_rpush(list, a);
    list_rpush(list, b);
    list_rpush(list, c);

    // Assertions
    assert(a == list_at(list, 0));
    assert(b == list_at(list, 1));
    assert(c == list_at(list, 2));
    assert(NULL == list_at(list, 3));

    assert(c == list_at(list, -1));
    assert(b == list_at(list, -2));
    assert(a == list_at(list, -3));
    assert(NULL == list_at(list, -4));

}

static void test_list_find() {
    // Setup
    list_t _langs;
    list_init(&_langs);

    list_node_t _js;
    list_node_t _ruby;
    list_node_create(&_js, "js");
    list_node_create(&_ruby, "ruby");

    list_t *langs = &_langs;
    list_node_t *js = list_rpush(langs, &_js);
    list_node_t *ruby = list_rpush(langs, &_ruby);

    list_t _users;
    list_init(&_users);
    list_t * users = &_users;
    users->match = (list_node_obj_match_t)User_equal;

    User userTJ = { "tj" };
    User userSimon = { "simon" };
    User userTaylor = { "taylor" };
    list_node_t _tj;
    list_node_t _simon;
    list_node_create(&_tj, &userTJ);
    list_node_create(&_simon, &userSimon);

    list_node_t *tj = list_rpush(users, &_tj);
    list_node_t *simon = list_rpush(users, &_simon);

    // Assertions
    list_node_t *a = list_find(langs, "js");
    list_node_t *b = list_find(langs, "ruby");
    list_node_t *c = list_find(langs, "foo");
    assert(js == a);
    assert(ruby == b);
    assert(NULL == c);

    a = list_find(users, &userTJ);
    b = list_find(users, &userSimon);
    c = list_find(users, &userTaylor);
    assert(tj == a);
    assert(simon == b);
    assert(NULL == c);

}

static void test_list_remove() {
    // Setup
    list_t _list;
    list_init(&_list);

    list_node_t _a;
    list_node_create(&_a, "a");
    list_node_t _b;
    list_node_create(&_b, "b");
    list_node_t _c;
    list_node_create(&_c, "c");

    list_t *list = &_list;
    list_node_t *a = list_rpush(list, &_a);
    list_node_t *b = list_rpush(list, &_b);
    list_node_t *c = list_rpush(list, &_c);
    // Assertions
    assert(3 == list->len);

    list_remove(list, b);
    assert(2 == list->len);
    assert(a == list->head);
    assert(c == list->tail);
    assert(c == a->next);
    assert(NULL == a->prev);
    assert(NULL == c->next);
    assert(a == c->prev);

    list_remove(list, a);
    assert(1 == list->len);
    assert(c == list->head);
    assert(c == list->tail);
    assert(NULL == c->next);
    assert(NULL == c->prev);

    list_remove(list, c);
    assert(0 == list->len);
    assert(NULL == list->head);
    assert(NULL == list->tail);
}

static void test_list_rpop() {
    // Setup
    list_t _list;
    list_init(&_list);

    list_node_t _a;
    list_node_create(&_a, "a");
    list_node_t _b;
    list_node_create(&_b, "b");
    list_node_t _c;
    list_node_create(&_c, "c");

    list_t *list = &_list;
    list_node_t *a = list_rpush(list, &_a);
    list_node_t *b = list_rpush(list, &_b);
    list_node_t *c = list_rpush(list, &_c);

    // Assertions
    assert(3 == list->len);

    assert(c == list_rpop(list));
    assert(2 == list->len);
    assert(a == list->head);
    assert(b == list->tail);
    assert(a == b->prev);
    assert(NULL == list->tail->next && "new tail node next is not NULL");
    assert(NULL == c->prev && "detached node prev is not NULL");
    assert(NULL == c->next && "detached node next is not NULL");

    assert(b == list_rpop(list));
    assert(1 == list->len);
    assert(a == list->head);
    assert(a == list->tail);

    assert(a == list_rpop(list));
    assert(0 == list->len);
    assert(NULL == list->head);
    assert(NULL == list->tail);

    assert(NULL == list_rpop(list));
    assert(0 == list->len);
}

static void test_list_lpop() {
    // Setup
    list_t _list;
    list_init(&_list);

    list_node_t _a;
    list_node_create(&_a, "a");
    list_node_t _b;
    list_node_create(&_b, "b");
    list_node_t _c;
    list_node_create(&_c, "c");

    list_t *list = &_list;
    list_node_t *a = list_rpush(list, &_a);
    list_node_t *b = list_rpush(list, &_b);
    list_node_t *c = list_rpush(list, &_c);;

    // Assertions
    assert(3 == list->len);

    assert(a == list_lpop(list));
    assert(2 == list->len);
    assert(b == list->head);
    assert(NULL == list->head->prev && "new head node prev is not NULL");
    assert(NULL == a->prev && "detached node prev is not NULL");
    assert(NULL == a->next && "detached node next is not NULL");

    assert(b == list_lpop(list));
    assert(1 == list->len);

    assert(c == list_lpop(list));
    assert(0 == list->len);
    assert(NULL == list->head);
    assert(NULL == list->tail);

    assert(NULL == list_lpop(list));
    assert(0 == list->len);

}

static void test_list_iterator_t() {
    // Setup
    list_t _list;
    list_init(&_list);

    list_node_t _tj, _taylor, _simon;
    list_node_create(&_tj, "tj");
    list_node_create(&_taylor, "taylor");
    list_node_create(&_simon, "simon");

    list_t *list = &_list;
    list_node_t *tj = &_tj;
    list_node_t *taylor =&_taylor;
    list_node_t *simon = &_simon;

    // tj taylor simon
    list_rpush(list, tj);
    list_rpush(list, taylor);
    list_rpush(list, simon);

    // Assertions

    // From head
    list_iterator_t it;
    list_iterator_create(&it, list, LIST_HEAD);

    list_node_t *a = list_iterator_next(&it);
    list_node_t *b = list_iterator_next(&it);
    list_node_t *c = list_iterator_next(&it);
    list_node_t *d = list_iterator_next(&it);

    assert(a == tj);
    assert(b == taylor);
    assert(c == simon);
    assert(NULL == d);

    // From tail
    list_iterator_create(&it, list, LIST_TAIL);
    list_node_t *a2 = list_iterator_next(&it);
    list_node_t *b2 = list_iterator_next(&it);
    list_node_t *c2 = list_iterator_next(&it);
    list_node_t *d2 = list_iterator_next(&it);

    assert(a2 == simon);
    assert(b2 == taylor);
    assert(c2 == tj);
    assert(NULL == d2);

}

int main(void) {
    printf("\nlist_t: %d\n", sizeof(list_t));
    printf("list_node_t: %d\n", sizeof(list_node_t));
    printf("list_iterator_t: %d\n\n", sizeof(list_iterator_t));
    test(list_node_create);
    test(list_rpush);
    test(list_lpush);
    test(list_find);
    test(list_at);
    test(list_remove);
    test(list_rpop);
    test(list_lpop);
    test(list_iterator_t);
    puts("... \x1b[32m100%\x1b[0m\n");
    return 0;
}
