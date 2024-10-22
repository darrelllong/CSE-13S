def gap(n):
    while n > 1:
        n = 1 if n <= 2 else 5 * n // 11
        yield n

def shell_sort(arr):
    for step in gap(len(arr)):
        for i in range(step, len(arr)):
            for j in range(i, step - 1, -step):
                if arr[j] < arr[j - step]:
                    arr[j], arr[j - step] = arr[j - step], arr[j]
    return

from random import randint

for _ in range(2000):
    arr = [randint(0, 100) for _ in range(100)]
    copy = arr[:]
    shell_sort(arr)
    assert(arr == sorted(copy))
