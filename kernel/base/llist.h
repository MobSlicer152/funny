#ifndef LLIST_H
#define LLIST_H

#include "heap.h"

void add_node(Bin_t *bin, Node_t *node);

void remove_node(Bin_t *bin, Node_t *node);

Node_t *get_best_fit(Bin_t *list, size_t size);
Node_t *get_last_node(Bin_t *list);

Node_t *next(Node_t *current);
Node_t *prev(Node_t *current);

#endif
