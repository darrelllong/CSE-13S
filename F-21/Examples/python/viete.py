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

def viete(ε):
    g, p = 0, 1
    a = sqrt(2)
    while abs(g - p) > ε:
        g, p = p, p * a / 2
        a = sqrt(2 + a)
    return 2 * 1 / p

if __name__ == '__main__':
    ε = 1.0
    while ε > 0:
        print(f"Viète({ε}) = {viete(ε)}")
        ε /= 10
