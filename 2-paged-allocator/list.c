#include <stdlib.h>
#include "list.h"

/**
 * Create list node with given data
 */
struct list_node *create_node(void *data) {
    struct list_node *node =
        (struct list_node *)malloc(sizeof(struct list_node));
    node->data = data;
    return node;
};

/**
 * Insert new list node after `prev` node with data as `data`
 */
struct list_node *insert_node(struct list_node *prev, void *data) {
    struct list_node *new_node = create_node(data);

    if (prev->next) {
        struct list_node *next = prev->next;
        prev->next = new_node;
        next->prev = new_node;
        new_node->prev = prev;
        new_node->next = next;
    } else {
        prev->next = new_node;
        new_node->prev = prev;
    }

    return new_node;
}

/**
 * Find first node, which gives `1` on a `predicate`
 */
struct list_node *find_node(
    struct list_node *root,
    char (*predicate)(void *)
) {
    struct list_node *node = root;

    while (node) {
        char accepts = (*predicate)(node->data);
        if (accepts) {
            return node;
        }
        node = node->next;
    }

    return NULL;
};

/**
 * Add new list node after `prev` node with data as `data`
 */
void remove_node(struct list_node *node) {
    if (node->prev && node->next) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    } else if (node->prev) {
        node->prev->next = NULL;
    } else if (node->next) {
        node->next->prev = NULL;
    }
}
