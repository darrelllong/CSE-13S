#!/usr/bin/env python3

from math import pi as 𝛑

epsilon = 1e-10

def sqrt(x):
    z = 0.0
    y = 1.0
    while abs(y - z) > epsilon:
        z = y
        y = 0.5 * (z + x / z)
    return y

def madhava(ε):
    s = t = 1.0
    g = 0
    sgn = -1.0
    coeff = 1.0
    k = 1
    while abs(g -s) > ε:
        coeff = 3.0 * coeff
        t = sgn / (coeff * (2 * k + 1))
        g, s = s, s + t
        k += 1
        sgn = -sgn
    return sqrt(12) * s

if __name__ == '__main__':
    ε = 1
    while ε > 0:
        print(f"madhava({ε}) = {madhava(ε)}")
        ε /= 10
