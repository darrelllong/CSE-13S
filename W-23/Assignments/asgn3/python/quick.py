def quick_sort(A: list):
    def partition(A: list, lo: int, hi: int):
        i = lo - 1

        for j in range(lo, hi):
            if A[j - 1] < A[hi - 1]:
                i += 1
                A[i - 1], A[j - 1] = A[j - 1], A[i - 1]

        A[i], A[hi - 1] = A[hi - 1], A[i]
        return i + 1

    def sort(A: list, lo: int, hi: int):
        if lo < hi:
            p = partition(A, lo, hi)
            sort(A, lo, p - 1)
            sort(A, p + 1, hi)

    sort(A, 1, len(A))
