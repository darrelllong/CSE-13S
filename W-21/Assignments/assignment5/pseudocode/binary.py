def binary_sort(arr):
    for i in range(1, len(arr)):
        value = arr[i]
        left = 0
        right = i

        while (left < right):
            mid = left + ((right - left) // 2)

            if value >= arr[mid]:
                left = mid + 1
            else:
                right = mid

        for j in range(i, left, -1):
            arr[j - 1], arr[j] = arr[j] , arr[j - 1]

    return

from random import randint

for _ in range(1000):
    arr = [randint(0, 100) for _ in range(200)]
    copy = arr[:]
    binary_sort(arr)
    assert(arr == sorted(copy))
