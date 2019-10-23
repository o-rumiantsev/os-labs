#ifndef LIST_H
#define LIST_H

struct list_node;

/**
 * Doubly linked list node structure
 *     *prev - previous node
 *     *next - next node
 *     *data - node data
 */
struct list_node {
    struct list_node *next;
    struct list_node *prev;
    void             *data;
};

/**
 * Create list node with given data
 */
struct list_node *create_node(void *data);

/**
 * Insert new list node after `prev` node with data as `data`
 */
struct list_node *insert_node(struct list_node *prev, void *data);


/**
 * Find first node, which gives `1` on a `predicate`
 */
struct list_node *find_node(struct list_node *root, char (*predicate)(void *));

/**
 * Remove list node `node`
 */
void remove_node(struct list_node *node);

#endif
