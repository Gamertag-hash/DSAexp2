"""k-way merge using a Min Heap (manual implementation so comparisons can be counted)."""


class MinHeap:
    """Min heap of (value, list_index, element_index) tuples, ordered by value."""

    def __init__(self):
        self.data = []
        self.comparisons = 0

    def __len__(self):
        return len(self.data)

    def _less(self, i, j):
        self.comparisons += 1
        return self.data[i][0] < self.data[j][0]

    def push(self, item):
        self.data.append(item)
        i = len(self.data) - 1
        while i > 0:
            parent = (i - 1) // 2
            if self._less(i, parent):
                self.data[i], self.data[parent] = self.data[parent], self.data[i]
                i = parent
            else:
                break

    def pop(self):
        top = self.data[0]
        last = self.data.pop()
        if self.data:
            self.data[0] = last
            self._sift_down(0)
        return top

    def _sift_down(self, i):
        n = len(self.data)
        while True:
            left, right, smallest = 2 * i + 1, 2 * i + 2, i
            if left < n and self._less(left, smallest):
                smallest = left
            if right < n and self._less(right, smallest):
                smallest = right
            if smallest == i:
                break
            self.data[i], self.data[smallest] = self.data[smallest], self.data[i]
            i = smallest

    def values(self):
        return [item[0] for item in self.data]


def kway_merge(lists, verbose=False):
    """Merge k sorted lists. Returns (merged, stats)."""
    heap = MinHeap()
    merged = []
    max_heap_size = 0
    pushes = pops = 0

    # Initialise heap with the first element of each list
    for li, lst in enumerate(lists):
        if lst:
            heap.push((lst[0], li, 0))
            pushes += 1
    max_heap_size = len(heap)
    if verbose:
        print(f"Initial heap (array form): {heap.values()}")

    step = 0
    while len(heap):
        val, li, ei = heap.pop()
        pops += 1
        merged.append(val)
        step += 1
        if ei + 1 < len(lists[li]):
            heap.push((lists[li][ei + 1], li, ei + 1))
            pushes += 1
        max_heap_size = max(max_heap_size, len(heap))
        if verbose:
            print(f"Step {step:2d}: extracted {val:>3} (from L{li + 1}) "
                  f"-> heap = {heap.values()}")

    stats = {
        "comparisons": heap.comparisons,
        "heap_operations": pushes + pops,
        "max_heap_size": max_heap_size,
    }
    return merged, stats
