#!/usr/bin/env python3

from math import pi as pi

def wallis(ε = 0.001):
    g, p, k = 0.0, 1.0, 1
    while abs(g - p) > ε:
        g, p = p, p * (4 * k**2) / (4 * k**2 - 1)
        k += 1
    return 2 * p

def pie(ε = 0.00001): return wallis(ε)

if __name__ == '__main__':
    for i in range(1, 15):
        print(f"𝛑 = {pie(10**-i)} (𝚫 = {pie(10**-i) - pi})")

