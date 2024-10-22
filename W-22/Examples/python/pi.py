#!/usr/bin/env python3

from math import exp

def f(x): return exp(-x * x)

def trapezoid(f, a, b, n):
    h = (b - a) / n
    sum = f(a) + f(b)
    for j in range(1, n):
        sum += 2 * f(a + j * h)
    sum *= h / 2.0
    return sum

if __name__ == '__main__':
    print(f"π = {trapezoid(f, -7, 7, 500)**2}")
