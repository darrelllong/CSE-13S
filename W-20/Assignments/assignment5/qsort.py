#!/usr/bin/python

from random import randint

def bubble(a):
    for i in range(len(a) - 1):
        j = len(a) - 1
        while j > i:
            if a[j] < a[j - 1]:
               a[j], a[j - 1] = a[j - 1], a[j]
            j -= 1
    return a

def gap(n):
    while n > 1:
        n = 1 if n <= 2 else 5 * n // 11
        yield(n)

def shellSort(s):
    for step in gap(len(s)):
        for i in range(step, len(s)):
            for j in range(i, step - 1, -step):
                if s[j] < s[j - step]:
                    s[j], s[j - step] = s[j - step], s[j]
    return s

def QSort(a):
    if len(a) < SMALL:
        return shellSort(a)
    else:
        pivot = (a[0] + a[len(a) / 2] + a[len(a) -1]) / 3
        left  = [ _ for _ in a if  _ <  pivot ]
        mid   = [ _ for _ in a if  _ == pivot ]
        right = [ _ for _ in a if  _ >  pivot ]
        return QSort(left) + mid + QSort(right)

SMALL = int(raw_input("How small? "))
n = int(raw_input("How many? "))

a = [ randint(1,n) for _ in range(n) ]

print "Unsorted: ", a[:10]
print "Sorted:   ", QSort(a)[:10]
