#ifndef HEAP_H
#define HEAP_H

#include "kernel/macros.h"
#include "kernel/types.h"

#define HEAP_INIT_SIZE 0x10000
#define HEAP_MAX_SIZE 0xF0000
#define HEAP_MIN_SIZE 0x10000

#define MIN_ALLOC_SZ 4

#define MIN_WILDERNESS 0x2000
#define MAX_WILDERNESS 0x1000000

#define BIN_COUNT 9
#define BIN_MAX_IDX (BIN_COUNT - 1)

typedef struct Node {
    uptr hole;
    uptr size;
    struct Node* next;
    struct Node* prev;
} Node_t;

typedef struct { 
    Node_t *header;
} Footer_t;

typedef struct {
    Node_t* head;
} Bin_t;

typedef struct {
    uptr start;
    uptr end;
    Bin_t *bins[BIN_COUNT];
} Heap_t;

static u32 overhead = sizeof(Footer_t) + sizeof(Node_t);

extern Heap_t g_kernelHeap;
#define GetKernelHeap() (&g_kernelHeap)

void InitializeHeap(Heap_t *heap, long start);

void *HeapAlloc(Heap_t *heap, size_t size);
void HeapFree(Heap_t *heap, void *p);
u32 expand(Heap_t *heap, size_t sz);
void contract(Heap_t *heap, size_t sz);

u32 get_bin_index(size_t sz);
void create_foot(Node_t *head);
Footer_t *get_foot(Node_t *head);

Node_t *get_wilderness(Heap_t *heap);

#endif
