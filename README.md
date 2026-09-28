# K-Way Merge (Min Heap) vs Pairwise Merging

Merging already-sorted transaction lists in a financial system, comparing two approaches.

```
L1 = 10, 30, 50, 70
L2 = 20, 40, 60, 80
L3 = 15, 35, 55, 75
```

## Files

| File | Purpose |
|------|---------|
| `kway_merge.py` | Manual Min Heap + k-way merge (counts comparisons) |
| `pairwise_merge.py` | Two-way merge applied one list at a time |
| `main.py` | Runs both, prints heap states, and a scaling experiment |

## Run

```bash
python main.py
```
No external dependencies (Python 3.8+).

## (a) Min Heap k-way merge

Each list is represented as a Python list with a pointer. The heap stores `(value, list_id, index)` and holds **at most one element per list**.

| Step | Extracted | Heap after (array form) |
|------|-----------|-------------------------|
| init | – | [10, 20, 15] |
| 1 | 10 (L1) | [15, 20, 30] |
| 2 | 15 (L3) | [20, 30, 35] |
| 3 | 20 (L2) | [30, 35, 40] |
| 4 | 30 (L1) | [35, 40, 50] |
| 5 | 35 (L3) | [40, 50, 55] |
| 6 | 40 (L2) | [50, 55, 60] |
| 7 | 50 (L1) | [55, 60, 70] |
| 8 | 55 (L3) | [60, 70, 75] |
| 9 | 60 (L2) | [70, 75, 80] |
| 10 | 70 (L1) | [75, 80] |
| 11 | 75 (L3) | [80] |
| 12 | 80 (L2) | [] |

**Output:** `10, 15, 20, 30, 35, 40, 50, 55, 60, 70, 75, 80`

## (b) Pairwise merging

1. `L1 + L2` → `10, 20, 30, 40, 50, 60, 70, 80` (7 comparisons)
2. `(L1+L2) + L3` → final list (11 comparisons)

## Measured results (this data, N = 12, k = 3)

| Metric | Min Heap | Pairwise |
|--------|----------|----------|
| Element comparisons | 21 | 18 |
| Heap operations (push + pop) | 24 | – |
| Element copies | 12 | 20 |
| Max heap size / extra space | 3 | 12 (intermediate list) |

For a tiny k = 3, pairwise does slightly **fewer** comparisons, because heap sift operations carry constant overhead. The picture flips as k grows.

## (c) Analysis

| Aspect | Min Heap k-way | Pairwise |
|--------|----------------|----------|
| Heap size | k (constant, independent of N) | n/a |
| Comparisons | O(N log k) | O(N·k) worst case (elements re-copied each round) |
| Time complexity | **O(N log k)** | **O(N·k)** |
| Space | O(k) heap + O(N) output | O(N) for intermediate lists |
| Passes over data | 1 | k − 1 |

Here N is total elements and k the number of lists.

### Scaling experiment (1000 elements per list, random data)

| k | Heap comparisons | Pairwise comparisons |
|---|------------------|----------------------|
| 2 | 1,999 | 1,999 |
| 4 | 14,291 | 8,997 |
| 8 | 46,845 | 34,966 |
| 16 | 127,422 | 134,932 |
| 32 | 316,825 | 526,641 |
| 64 | 753,607 | 2,077,246 |

The crossover is around k ≈ 16. Beyond that the gap widens rapidly, following N log k vs N·k.

### Conclusion

When the number of sorted files **increases**, the **Min Heap k-way merge is more suitable**: it makes a single pass, needs only O(k) working memory, and scales as O(N log k) instead of O(N·k). It also fits external merging (large files streamed from disk). Pairwise merging is fine for very small k due to its simplicity and low constant factors. A balanced (tournament-style) pairwise merge would also reach O(N log k), but still needs intermediate storage.
