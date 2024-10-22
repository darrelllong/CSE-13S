import random, sys

def partition(A, lo, hi):
    pivot = A[(lo + hi) // 2]
    i, j = lo, hi

    while i < j:
        while A[i] < pivot:
            i += 1
        while A[j] > pivot:
            j -= 1
        if i < j:
            A[i], A[j] = A[j], A[i]

    return j

def quick_sort(A):
    left, right = 0, len(A) - 1

    stack = []
    stack.append(left)
    stack.append(right)

    while len(stack) != 0:
        hi = stack.pop()
        lo = stack.pop()
        p = partition(A, lo, hi)
        if p + 1 < hi:
            stack.append(p + 1)
            stack.append(hi)
        if lo < p:
            stack.append(lo)
            stack.append(p)


for n in range(2, 1001):
    print(f"Sorting {n} elements... ", end='')
    arr = [random.randint(0, sys.maxsize) for _ in range(n)]
    quick_sort(arr)

    for i in range(1, len(arr)):
        if arr[i] < arr[i - 1]:
            print("failed")
            break
    else:
        print("passed")
