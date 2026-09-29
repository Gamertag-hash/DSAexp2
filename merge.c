#ifndef MERGE_H
#define MERGE_H
#include <stddef.h>

typedef struct { const int *data; size_t len; } List;

typedef struct { long comparisons, heap_operations; size_t max_heap_size; } KwayStats;
typedef struct { long comparisons, element_copies; size_t max_intermediate_size; } PairStats;

/* Both return a malloc'd array of total length N; caller must free(). */
int *kway_merge(const List *lists, size_t k, KwayStats *stats, int verbose);
int *pairwise_merge(const List *lists, size_t k, PairStats *stats, int verbose);
#endif
