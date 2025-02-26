#include "heap.h"
#include "llist.h"

u32 offset = 8;

Heap_t g_kernelHeap;

void InitializeHeap(Heap_t *heap, long start) {
    Node_t *init_region = (Node_t *) start;
    init_region->hole = 1;
    init_region->size = (HEAP_INIT_SIZE) - sizeof(Node_t) - sizeof(Footer_t);

    create_foot(init_region);

    add_node(heap->bins[get_bin_index(init_region->size)], init_region);

    heap->start = (uptr)start;
    heap->end   = (uptr)(start + HEAP_INIT_SIZE);
}

void *HeapAlloc(Heap_t *heap, usize size) {
    u32 index = get_bin_index(size);
    Bin_t *temp = (Bin_t *) heap->bins[index];
    Node_t *found = get_best_fit(temp, size);

    while (found == NULL) {
        if (index + 1 >= BIN_COUNT)
            return NULL;

        temp = heap->bins[++index];
        found = get_best_fit(temp, size);
    }

    if ((found->size - size) > (overhead + MIN_ALLOC_SZ)) {
        Node_t *split = (Node_t *) (((char *) found + sizeof(Node_t) + sizeof(Footer_t)) + size);
        split->size = found->size - size - sizeof(Node_t) - sizeof(Footer_t);
        split->hole = 1;
   
        create_foot(split);

        u32 new_idx = get_bin_index(split->size);

        add_node(heap->bins[new_idx], split); 

        found->size = size; 
        create_foot(found); 
    }

    found->hole = 0; 
    remove_node(heap->bins[index], found); 
    
    Node_t *wild = get_wilderness(heap);
    if (wild->size < MIN_WILDERNESS) {
        u32 success = expand(heap, 0x1000);
        if (success == 0) {
            return NULL;
        }
    }
    else if (wild->size > MAX_WILDERNESS) {
        contract(heap, 0x1000);
    }

    found->prev = NULL;
    found->next = NULL;
    return &found->next; 
}

void HeapFree(Heap_t *heap, void *p) {
    Bin_t *list;
    Footer_t *new_foot, *old_foot;

    Node_t *head = (Node_t *)((char *) p - offset);
    if (head == (Node_t *)(uptr)heap->start) {
        head->hole = 1; 
        add_node(heap->bins[get_bin_index(head->size)], head);
        return;
    }

    Node_t *next = (Node_t *) ((char *) get_foot(head) + sizeof(Footer_t));
    Footer_t *f = (Footer_t *) ((char *) head - sizeof(Footer_t));
    Node_t *prev = f->header;
    
    if (prev->hole) {
        list = heap->bins[get_bin_index(prev->size)];
        remove_node(list, prev);

        prev->size += overhead + head->size;
        new_foot = get_foot(head);
        new_foot->header = prev;

        head = prev;
    }

    if (next->hole) {
        list = heap->bins[get_bin_index(next->size)];
        remove_node(list, next);

        head->size += overhead + next->size;

        old_foot = get_foot(next);
        old_foot->header = 0;
        next->size = 0;
        next->hole = 0;
        
        new_foot = get_foot(head);
        new_foot->header = head;
    }

    head->hole = 1;
    add_node(heap->bins[get_bin_index(head->size)], head);
}

u32 expand(Heap_t *heap, usize sz) {
    UNUSED(heap);
    UNUSED(sz);
    return 0;
}

void contract(Heap_t *heap, usize sz) {
    UNUSED(heap);
    UNUSED(sz);
    return;
}

u32 get_bin_index(usize sz) {
    u32 index = 0;
    sz = sz < 4 ? 4 : sz;

    while (sz >>= 1) index++; 
    index -= 2; 
    
    if (index > BIN_MAX_IDX) index = BIN_MAX_IDX; 
    return index;
}

void create_foot(Node_t *head) {
    Footer_t *foot = get_foot(head);
    foot->header = head;
}

Footer_t *get_foot(Node_t *node) {
    return (Footer_t *) ((char *) node + sizeof(Node_t) + node->size);
}

Node_t *get_wilderness(Heap_t *heap) {
    Footer_t *wild_foot = (Footer_t *) ((char *) heap->end - sizeof(Footer_t));
    return wild_foot->header;
}
