/* Runs both approaches on the sample data and on larger k to show scaling. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "merge.h"

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}
static void print_arr(const int *a, size_t n) {
    printf("[");
    for (size_t i = 0; i < n; i++) printf(i ? ", %d" : "%d", a[i]);
    printf("]\n");
}

int main(void) {
    static const int L1[] = {10, 30, 50, 70};
    static const int L2[] = {20, 40, 60, 80};
    static const int L3[] = {15, 35, 55, 75};
    List lists[] = { {L1, 4}, {L2, 4}, {L3, 4} };
    size_t k = 3, N = 12;

    puts("============================================================");
    puts("(a) k-way merge with Min Heap");
    puts("============================================================");
    KwayStats hs;
    int *hr = kway_merge(lists, k, &hs, 1);
    printf("Result: "); print_arr(hr, N);
    printf("Stats : comparisons=%ld, heap_operations=%ld, max_heap_size=%zu\n",
           hs.comparisons, hs.heap_operations, hs.max_heap_size);

    puts("\n============================================================");
    puts("(b) Pairwise merging");
    puts("============================================================");
    PairStats ps;
    int *pr = pairwise_merge(lists, k, &ps, 1);
    printf("Result: "); print_arr(pr, N);
    printf("Stats : comparisons=%ld, element_copies=%ld, max_intermediate_size=%zu\n",
           ps.comparisons, ps.element_copies, ps.max_intermediate_size);

    for (size_t i = 0; i < N; i++)
        if (hr[i] != pr[i] || (i && hr[i] < hr[i - 1])) { puts("MISMATCH!"); return 1; }
    free(hr); free(pr);

    puts("\n============================================================");
    puts("(c) Scaling: n = 1000 elements per list");
    puts("============================================================");
    printf("%5s | %10s | %13s | %10s\n", "k", "heap cmp", "pairwise cmp", "N log2 k");
    srand(0);
    const size_t per = 1000;
    for (size_t kk = 2; kk <= 64; kk *= 2) {
        int **bufs = malloc(kk * sizeof(int *));
        List *big = malloc(kk * sizeof(List));
        for (size_t i = 0; i < kk; i++) {
            bufs[i] = malloc(per * sizeof(int));
            for (size_t j = 0; j < per; j++) bufs[i][j] = rand() % 1000000;
            qsort(bufs[i], per, sizeof(int), cmp_int);
            big[i].data = bufs[i]; big[i].len = per;
        }
        KwayStats h; PairStats p;
        int *a = kway_merge(big, kk, &h, 0);
        int *b = pairwise_merge(big, kk, &p, 0);
        printf("%5zu | %10ld | %13ld | %10d\n", kk, h.comparisons, p.comparisons,
               (int)(per * kk * log2((double)kk)));
        free(a); free(b);
        for (size_t i = 0; i < kk; i++) free(bufs[i]);
        free(bufs); free(big);
    }
    return 0;
}
