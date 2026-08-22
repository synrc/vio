#ifndef UTIL_LIST_H
#define UTIL_LIST_H

#include <stddef.h>

/**
 * Minimal intrusive doubly-linked list.
 */

struct vio_list_node {
    struct vio_list_node *next;
    struct vio_list_node *prev;
};

struct vio_list {
    struct vio_list_node head;
};

static inline void vio_list_init(struct vio_list *list) {
    list->head.next = &list->head;
    list->head.prev = &list->head;
}

static inline void vio_list_insert_after(struct vio_list_node *pos, struct vio_list_node *node) {
    node->next = pos->next;
    node->prev = pos;
    pos->next->prev = node;
    pos->next = node;
}

static inline void vio_list_add_tail(struct vio_list *list, struct vio_list_node *node) {
    vio_list_insert_after(list->head.prev, node);
}

static inline void vio_list_remove(struct vio_list_node *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = node->prev = NULL;
}

static inline int vio_list_is_empty(const struct vio_list *list) {
    return list->head.next == &list->head;
}

#endif /* UTIL_LIST_H */
