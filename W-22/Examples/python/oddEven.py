#!/usr/bin/env python3

from random import *

def oddEvenMerge(a, low, n, r):
    m = 2 * r
    if m < n:
        oddEvenMerge(a, low, n, m)
        oddEvenMerge(a, low + r, n, m)
        for i in range(low + r, low + n - r, m):
            if a[i] > a[i + r]:
                a[i], a[i + r] = a[i + r], a[i]
    else:
        if a[low] > a[low + r]:
            a[low], a[low + r] = a[low + r], a[low]

def oddEvenMergeSort(a, low, n):
    if n > 1:
        m = n // 2
        oddEvenMergeSort(a, low, m)
        oddEvenMergeSort(a, low + m, m)
        oddEvenMerge(a, low, n, 1)

def mergeSort(a):
    oddEvenMergeSort(a, 0, len(a))

def main():
    a = [ randint(1,20) for _ in range(32) ]
    print(a)
    mergeSort(a)
    print(a)

if __name__ == '__main__':
    main()
