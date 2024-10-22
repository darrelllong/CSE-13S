#!/usr/bin/env python3

def a(x): return sqrt(1 - x**4)

def b(x): return 1/log(x)

def c(x): return exp(-x**2)

def d(x): return sin(x**2)

def e(x): return cos(x**2)

def f(x): return log(log(x))

def g(x): return sin(x)/x

def h(x): return exp(-x)/x

def i(x): return exp(exp(x))

from integrate import *

import sys

from math import pi as π

if __name__ == '__main__':
    if "-t" in sys.argv:
        integrate = trapezoid
    elif "-h" == sys.argv:
        integrate = homer
    elif "-b" == sys.argv:
        integrate = bart
    elif "-B" == sys.argv:
        integrate = boole
    else:
        integrate = simpson

    y = integrate(a, 0, 1.0, 100)
    print(f"∫sqrt(1 - x**4) = {y}")

    y = integrate(b, 2.0, 3.0, 100)
    print(f"∫1/log(x) = {y}")

    y = integrate(c, -7.0, 7.0, 100)
    print(f"∫exp(-x**2) = {y}, y**2 = {y*y}")

    y = integrate(d, -π, π, 100)
    print(f"∫sin(x**2) = {y}")

    y = integrate(e, -π, π, 100)
    print(f"∫cos(x**2) = {y}")

    y = integrate(f, 2.00, 10.0, 100)
    print(f"∫log(log(x)) = {y}")

    y = integrate(g, -4 * π, 4 * π, 100)
    print(f"∫sin(x)/x = {y}")

    y = integrate(h, 1.0, 10.0, 100)
    print(f"∫exp(-x)/x = {y}")

    y = integrate(i, 0.0, 1.0, 100)
    print(f"∫exp(exp(x)) = {y}")

    if "-v" in sys.argv:
        if "-b" in sys.argv:
            for h in range(3, 202, 3):
                y = integrate(c, -20, 20, h)
                print(f"[∫ e^(-x^2)]^2 = {y*y} ({h} terms)")
            ℇ = 1e-14
            for h in range(3, 202, 3):
                y = 2 * integrate(g, ℇ, 4 * π, h)
                print(f"[∫ sin(x)/x = {y} ({h} terms)")
        else:
            for h in range(2, 202, 2):
                y = integrate(c, -20, 20, h)
                print(f"[∫ e^(-x^2)]^2 = {y*y} ({h} terms)")
            ℇ = 1e-14
            for h in range(2, 202, 2):
                y = 2 * integrate(g, ℇ, 4 * π, h)
                print(f"[∫ sin(x)/x = {y} ({h} terms)")
