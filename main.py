"""Runs both approaches on the question's data and on larger k to show scaling."""
import math
import random

from kway_merge import kway_merge
from pairwise_merge import pairwise_merge

L1 = [10, 30, 50, 70]
L2 = [20, 40, 60, 80]
L3 = [15, 35, 55, 75]


def main():
    lists = [L1, L2, L3]
    print("=" * 60)
    print("(a) k-way merge with Min Heap")
    print("=" * 60)
    heap_result, heap_stats = kway_merge(lists, verbose=True)
    print("Result:", heap_result)
    print("Stats :", heap_stats)

    print()
    print("=" * 60)
    print("(b) Pairwise merging")
    print("=" * 60)
    pair_result, pair_stats = pairwise_merge(lists, verbose=True)
    print("Result:", pair_result)
    print("Stats :", pair_stats)

    assert heap_result == pair_result == sorted(sum(lists, []))

    print()
    print("=" * 60)
    print("(c) Scaling: n = 1000 elements per list")
    print("=" * 60)
    print(f"{'k':>5} | {'heap cmp':>10} | {'pairwise cmp':>13} | {'N log2 k':>10}")
    random.seed(0)
    for k in (2, 4, 8, 16, 32, 64):
        big = [sorted(random.sample(range(10**6), 1000)) for _ in range(k)]
        _, h = kway_merge(big)
        _, p = pairwise_merge(big)
        n_total = 1000 * k
        print(f"{k:>5} | {h['comparisons']:>10} | {p['comparisons']:>13} | "
              f"{int(n_total * math.log2(k)):>10}")


if __name__ == "__main__":
    main()
