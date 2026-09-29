/* Simple pairwise (two-way) merging: ((L1 + L2) + L3) + ... */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merge.h"

static int *merge_two(const int *a, size_t na, const int *b, size_t nb,
                      long *cmp, long *copies) {
    int *out = malloc((na + nb ? na + nb : 1) * sizeof(int));
    size_t i = 0, j = 0, o = 0;
    while (i < na && j < nb) {
        (*cmp)++;
        out[o++] = (a[i] <= b[j]) ? a[i++] : b[j++];
        (*copies)++;
    }
    while (i < na) { out[o++] = a[i++]; (*copies)++; }
    while (j < nb) { out[o++] = b[j++]; (*copies)++; }
    return out;
}

int *pairwise_merge(const List *lists, size_t k, PairStats *stats, int verbose) {
    size_t len = lists[0].len;
    int *result = malloc((len ? len : 1) * sizeof(int));
    memcpy(result, lists[0].data, len * sizeof(int));
    long total_cmp = 0, total_copies = 0;
    size_t max_extra = 0;

    for (size_t idx = 1; idx < k; idx++) {
        long c = 0, cp = 0;
        int *next = merge_two(result, len, lists[idx].data, lists[idx].len, &c, &cp);
        free(result);
        result = next;
        len += lists[idx].len;
        total_cmp += c; total_copies += cp;
        if (len > max_extra) max_extra = len;
        if (verbose) {
            printf("After merging L%zu: [", idx + 1);
            for (size_t i = 0; i < len; i++) printf(i ? ", %d" : "%d", result[i]);
            printf("]  (comparisons=%ld, copies=%ld)\n", c, cp);
        }
    }
    stats->comparisons = total_cmp;
    stats->element_copies = total_copies;
    stats->max_intermediate_size = max_extra;
    return result;
}
