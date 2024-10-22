#!/usr/bin/env python3

from math import pi as 𝛑

epsilon = 1e-10

def leibniz(ε):
    s, g, p, k = 1, 1, 0, 0
    while abs(g - p) > ε:
        g, p = p, p + s/(2 * k + 1)
        s *= -1
        k += 1
    return 4 * p

if __name__ == '__main__':
    ε = 1.0
    while ε > 1e-9:
        print(f"Leibniz({ε}) = {leibniz(ε)}")
        ε /= 10
