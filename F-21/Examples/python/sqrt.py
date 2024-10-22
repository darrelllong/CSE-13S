#!/usr/bin/env python3

def sqrt(x, ε = 1e-14):
    z = 0.0
    y = 1.0
    while abs(y - z) > ε:
        z = y
        y = 0.5 * (z + x / z)
    return y

for x in [ x * 0.01 for x in range(1, 101) ]:
    print(f"sqrt({x}) = {sqrt(x)}")
