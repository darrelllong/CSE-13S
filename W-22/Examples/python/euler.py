#!/usr/bin/env python3

from math import pi as 𝛑

epsilon = 1e-10

def sqrt(x):
    z = 0.0
    y = 1.0
    while abs(y - z) > epsilon:
        z = y
        y = (z + x / z) / 2
    return y

def euler(ε):
    g, p, k = 0, 1, 2
    while abs(g - p) > ε:
        g, p = p, p + 1/k**2
        k += 1
    return sqrt(6 * p)

if __name__ == '__main__':
    ε = 1.0
    while ε > 1e-15:
        print(f"Euler({ε}) = {euler(ε)}")
        ε /= 10
