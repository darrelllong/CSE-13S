#!/usr/bin/env python3

from math import factorial as factorial
from math import pi as pi

def plouffe(n):
    return sum(map(lambda k : k * (2**k *factorial(k)**2)/factorial(2 * k), range(1,n))) - 3

if __name__ == '__main__':
    for n in range(0, 101):
        print(f"𝛑({n}) = {plouffe(n)} \t(𝚫 = {plouffe(n) - pi})")
