#include "llist.h"

void add_node(Bin_t *bin, Node_t* node) {
    node->next = NULL;
    node->prev = NULL;

    if (bin->head == NULL) {
        bin->head = node;
        return;
    }
    
    // we need to save next and prev while we iterate
    Node_t *current = bin->head;
    Node_t *previous = NULL;
    // iterate until we get the the end of the list or we find a 
    // node whose size is
    while (current != NULL && current->size <= node->size) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) { // we reached the end of the list
        previous->next = node;
        node->prev = previous;
    }
    else {
        if (previous != NULL) { // middle of list, connect all links!
            node->next = current;
            previous->next = node;

            node->prev = previous;
            current->prev = node;
        }
        else { // head is the only element
            node->next = bin->head;
            bin->head->prev = node;
            bin->head = node;
        }
    }
}

void remove_node(Bin_t * bin, Node_t *node) {
    if (bin->head == NULL) return; 
    if (bin->head == node) { 
        bin->head = bin->head->next;
        return;
    }
    
    Node_t *temp = bin->head->next;
    while (temp != NULL) {
        if (temp == node) { // found the node
            if (temp->next == NULL) { // last item
                temp->prev->next = NULL;
            }
            else { // middle item
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
            }
            // we dont worry about deleting the head here because we already checked that
            return;
        }
        temp = temp->next;
    }
}

Node_t *get_best_fit(Bin_t *bin, size_t size) {
    if (bin->head == NULL) return NULL; // empty list!

    Node_t *temp = bin->head;

    while (temp != NULL) {
        if (temp->size >= size) {
            return temp; // found a fit!
        }
        temp = temp->next;
    }
    return NULL; // no fit!
}

Node_t *get_last_node(Bin_t *bin) {
    Node_t *temp = bin->head;

    while (temp->next != NULL) {
        temp = temp->next;
    }
    return temp;
}

