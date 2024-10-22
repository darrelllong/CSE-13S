#!/usr/bin/env python3

def integrate(f, a, b, n):
    h = (b - a) / n
    sum = f(a)
    tmp = 0.0
    j = 1
    while j <= n // 2 - 1:
        tmp += f(a + 2 * j * h)
        j += 1
    sum += 2 * tmp
    tmp = 0.0
    j = 1
    while j <= n // 2:
        tmp += f(a + (2 * j - 1) * h)
        j += 1
    sum += 4.0 * tmp
    sum += f(b)
    sum *= h / 3.0
    return sum

from math import exp as exp

def g(x):
        return exp(-x * x)

from math import sqrt as sqrt

def h(x):
        return sqrt(1 - x * x)

def l(x):
        return sqrt(x) * exp(-x)

from math import pi as pi

if __name__ == '__main__':
    p = integrate(g, -7.0, 7.0, 100)
    𝛑 = p * p
    print(f"𝛑 = {𝛑} (𝚫 = {pi - 𝛑})")
    𝛑 = 2 * integrate(h, -1.0, 1.0, 5000000)
    print(f"𝛑 = {𝛑} (𝚫 = {pi - 𝛑})")
    p = integrate(l, 0.0, 33.0, 10000000)
    𝛑 = 4 * p * p
    print(f"𝛑 = {𝛑} (𝚫 = {pi - 𝛑})")
