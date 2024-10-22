#!/usr/bin/env python3

from math import floor as floor

#  -x        x
# e   = 1 / e

error = 1e-15

def exp(x, ε = error):
    trm = 1.0
    sum = trm
    k = 1
    while trm > ε:
        trm *= abs(x) / k
        sum += trm
        k += 1
    return sum if x > 0 else 1 / sum

def log(x, ε = error):
    y = 1.0
    f = 0.0
    e = 2.7182818284590455 # Euler's constant
    while x > e:
        f += 1.0
        x /= e
    p = exp(y)
    while abs(p - x) > ε:
        y = y + x / p - 1
        p = exp(y)
    return y + f

def pow(x, y, ε = error): return exp(y * log(x, ε), ε)

def sqrt(x, ε = error):
    f = 1.0
    while x > 4:
        x /= 4.0
        f *= 2.0
    z = 0.0
    y = 1.0
    while abs(y - z) > ε:
        z = y
        y = y - (y**2 - x) / (2 * y)
    return y * f

def modulus(n, m):
    return n - m * (int) (n / m)

from math import pi as π

def sin(x, ε = error):
    x = modulus(x, 2 * π)
    s, v, t, k = 1.0, x, x, 3.0
    while abs(t) > ε:
        t = t * (x * x) / ((k - 1) * k)
        s = -s
        v += s * t
        k += 2.0
    return v

def cos(x, ε = error):
    x = modulus(x, 2 * π)
    s, v, t, k = 1.0, 1.0, 1.0, 1.0
    while abs(t) > ε:
        t = t * (x * x) / ((k + 1) * k)
        s = -s
        v += s * t
        k += 2.0
    return v

def trapezoid(f, a, b, n):
    h = (b - a) / n
    sum = f(a) + f(b)
    for j in range(1, n):
        sum += 2 * f(a + j * h)
    sum *= h / 2.0
    return sum

def simpson(f, a, b, n):
    h = (b - a) / n
    sum = f(a) + f(b)
    for j in range(2, n, 2): sum += 2 * f(a + j * h)
    for j in range(1, n, 2): sum += 4 * f(a + j * h)
    sum *= h / 3.0
    return sum

def even(n): return n & 0x1 == 0

def homer(f, a, b, n):
    h = (b - a) / n
    sum = f(a) + f(b)
    for j in range(1, n):
        sum += 2.0 * f(a + j * h) if even(j) else 4.0 * f(a + j * h)
    sum *= h / 3.0
    return sum

def bart(f, a, b, n):
    h = (b - a) / n
    sum = f(a) + f(b)
    for i in range(1, n):
        sum += 3.0 * f(a + i * h) if i % 3 != 0 else 2.0 * f(a + i * h)
    sum *= h * 3.0 / 8.0
    return sum

def boole(f, a, b, n):
    h = (b - a) / n
    sum = 7.0 * (f(a) + f(b))
    for i in range(1, n, 2):     sum += 32.0 * f(a + i * h)
    for i in range(2, n - 1, 4): sum += 12.0 * f(a + i * h)
    for i in range(4, n - 3, 4): sum += 14.0 * f(a + i * h)
    sum *= h * 2.0 / 45.0
    return sum
