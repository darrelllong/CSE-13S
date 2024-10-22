def partition(arr, left, right):
    pivot = arr[left]
    lo = left + 1
    hi = right

    while True:
        while lo <= hi and arr[hi] >= pivot:
            hi -= 1

        while lo <= hi and arr[lo] <= pivot:
            lo += 1

        if lo <= hi:
            arr[lo], arr[hi] = arr[hi], arr[lo]
        else:
            break

    arr[left], arr[hi] = arr[hi], arr[left]

    return hi


def quick_sort(arr, left, right):
    if left >= right:
        return

    index = partition(arr, left, right)
    quick_sort(arr, left, index - 1)
    quick_sort(arr, index + 1, right)


from random import randint

for _ in range(2000):
    arr = [randint(0, 100) for _ in range(100)]
    copy = arr[:]
    quick_sort(arr, 0, len(arr) - 1)
    assert(arr == sorted(copy))


