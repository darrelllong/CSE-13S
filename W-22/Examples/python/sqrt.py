#!/usr/bin/env python3

error = 1e-16

def sqrt(x, ε = error):
    f = 1.0
    while x > 1:
        x /= 4.0
        f *= 2.0
    z = 0.0
    y = 1.0
    while abs(y - z) > ε:
        z = y
        y = 0.5 * (z + x / z)
    return y * f

for x in [ x * 0.01 for x in range(1, 101) ]:
    print(f"sqrt({x}) = {sqrt(x)}")
