def bubble_sort(arr):
    for i in range(len(arr) - 1):
        j = len(arr) - 1

        while j > i:
            if arr[j] < arr[j - 1]:
                arr[j], arr[j - 1] = arr[j - 1], arr[j]

            j -= 1

    return

from random import randint

for _ in range(2000):
    arr = [randint(0, 100) for _ in range(100)]
    copy = arr[:]
    bubble_sort(arr)
    assert(arr == sorted(copy))
