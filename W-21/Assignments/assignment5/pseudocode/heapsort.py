import random, sys

def max_child(A, first, last):
    left = first * 2
    right = left + 1
    child = left

    if right <= last and A[right] > A[left]:
        child = right

    return child

def fix_heap(A, first, last):
    found = False
    father = first
    great = max_child(A, father, last)

    while father <= last // 2 and not found:
        if A[father] < A[great]:
            A[father], A[great] = A[great], A[father]
            father = great
            great = max_child(A, father, last)
        else:
            found = True

def build_heap(A, first, last):
    for father in range(last // 2, first - 1, -1):
        fix_heap(A, father, last)

def heap_sort(A):
    first, last = 0, len(A) - 1
    build_heap(A, first, last)
    for leaf in range(last, first, -1):
        A[first], A[leaf] = A[leaf], A[first]
        fix_heap(A, first, leaf - 1)

for n in range(2, 1001):
    print(f"Sorting {n} elements... ", end='')
    arr = [random.randint(0, sys.maxsize) for _ in range(n)]
    heap_sort(arr)

    for i in range(1, len(arr)):
        if arr[i] < arr[i - 1]:
            print("failed")
            break
    else:
        print("passed")
