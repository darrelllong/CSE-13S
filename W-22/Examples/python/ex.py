#!/usr/bin/env python3

from math import exp as exp

def exp(x, n = 24):
    term = 1.0
    sum  = term
    for k in range(1, n):
        term *= x/k
        sum  += term
    return sum

def e(n = 25): return exp(1, n)

if __name__ == '__main__':
    for i in range(1, 25):
        print(f"e = {e(i)} (𝚫 = {e(i) - exp(1)})")

