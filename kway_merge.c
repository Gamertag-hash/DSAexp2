/* k-way merge using a manual Min Heap (so comparisons can be counted). */
#include <stdio.h>
#include <stdlib.h>
#include "merge.h"

typedef struct { int val; size_t li, ei; } Item;
typedef struct { Item *data; size_t size; long comparisons; } MinHeap;

static void heap_init(MinHeap *h, size_t cap) {
    h->data = malloc((cap ? cap : 1) * sizeof(Item));
    h->size = 0; h->comparisons = 0;
}
static int less(MinHeap *h, size_t i, size_t j) {
    h->comparisons++;
    return h->data[i].val < h->data[j].val;
}
static void swap(Item *a, Item *b) { Item t = *a; *a = *b; *b = t; }

static void heap_push(MinHeap *h, Item it) {
    size_t i = h->size++;
    h->data[i] = it;
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (less(h, i, parent)) { swap(&h->data[i], &h->data[parent]); i = parent; }
        else break;
    }
}
static void sift_down(MinHeap *h, size_t i) {
    for (;;) {
        size_t l = 2 * i + 1, r = 2 * i + 2, s = i;
        if (l < h->size && less(h, l, s)) s = l;
        if (r < h->size && less(h, r, s)) s = r;
        if (s == i) break;
        swap(&h->data[i], &h->data[s]);
        i = s;
    }
}
static Item heap_pop(MinHeap *h) {
    Item top = h->data[0];
    h->size--;
    if (h->size) { h->data[0] = h->data[h->size]; sift_down(h, 0); }
    return top;
}
static void print_heap(const MinHeap *h) {
    printf("[");
    for (size_t i = 0; i < h->size; i++) printf(i ? ", %d" : "%d", h->data[i].val);
    printf("]");
}

int *kway_merge(const List *lists, size_t k, KwayStats *stats, int verbose) {
    size_t total = 0;
    for (size_t i = 0; i < k; i++) total += lists[i].len;
    int *merged = malloc((total ? total : 1) * sizeof(int));
    size_t n = 0;
    MinHeap heap; heap_init(&heap, k);
    long pushes = 0, pops = 0;

    for (size_t li = 0; li < k; li++)
        if (lists[li].len) {
            Item it = { lists[li].data[0], li, 0 };
            heap_push(&heap, it); pushes++;
        }
    size_t max_size = heap.size;
    if (verbose) { printf("Initial heap (array form): "); print_heap(&heap); printf("\n"); }

    int step = 0;
    while (heap.size) {
        Item top = heap_pop(&heap); pops++;
        merged[n++] = top.val; step++;
        if (top.ei + 1 < lists[top.li].len) {
            Item nx = { lists[top.li].data[top.ei + 1], top.li, top.ei + 1 };
            heap_push(&heap, nx); pushes++;
        }
        if (heap.size > max_size) max_size = heap.size;
        if (verbose) {
            printf("Step %2d: extracted %3d (from L%zu) -> heap = ", step, top.val, top.li + 1);
            print_heap(&heap); printf("\n");
        }
    }
    stats->comparisons = heap.comparisons;
    stats->heap_operations = pushes + pops;
    stats->max_heap_size = max_size;
    free(heap.data);
    return merged;
}
