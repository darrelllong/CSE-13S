def heap_sort(A: list):
    def max_child(A: list, first: int, last: int):
        left = first * 2
        right = left + 1
        child = left
        if right <= last and A[right - 1] > A[left - 1]:
            child = right;
        return child;

    def fix_heap(A: list, first: int, last: int):
        found = False
        mother = first
        great = max_child(A, mother, last)

        while mother <= last / 2 and not found:
            if A[mother - 1] < A[great - 1]:
                A[mother - 1], A[great - 1] = A[great - 1], A[mother - 1]
                mother = great;
                great = max_child(A, mother, last);
            else:
                found = True;

    def build_heap(A: list, first: int, last: int):
        for mother in range(last // 2, first - 1, -1):
            fix_heap(A, mother, last)

    first = 1
    last = len(A)
    build_heap(A, first, last)
    for leaf in range(last, first - 1, -1):
        A[first - 1], A[leaf - 1] = A[leaf - 1], A[first - 1]
        fix_heap(A, first, leaf - 1)
