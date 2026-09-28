"""Simple pairwise (two-way) merging: ((L1 + L2) + L3) + ..."""


def merge_two(a, b):
    """Standard two-way merge. Returns (merged, comparisons, copies)."""
    i = j = comparisons = copies = 0
    out = []
    while i < len(a) and j < len(b):
        comparisons += 1
        if a[i] <= b[j]:
            out.append(a[i]); i += 1
        else:
            out.append(b[j]); j += 1
        copies += 1
    while i < len(a):
        out.append(a[i]); i += 1; copies += 1
    while j < len(b):
        out.append(b[j]); j += 1; copies += 1
    return out, comparisons, copies


def pairwise_merge(lists, verbose=False):
    """Merge lists one after another. Returns (merged, stats)."""
    result = list(lists[0])
    total_cmp = total_copies = 0
    max_extra_space = 0
    for idx, nxt in enumerate(lists[1:], start=2):
        result, c, cp = merge_two(result, nxt)
        total_cmp += c
        total_copies += cp
        max_extra_space = max(max_extra_space, len(result))
        if verbose:
            print(f"After merging L{idx}: {result}  (comparisons={c}, copies={cp})")
    stats = {
        "comparisons": total_cmp,
        "element_copies": total_copies,
        "max_intermediate_size": max_extra_space,
    }
    return result, stats
