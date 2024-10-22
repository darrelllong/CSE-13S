#!/usr/bin/env python3

def divisors(n):
    return [ _ for _ in range(1, n) if n % _ == 0 ]

def combination(d):
    for c in range(2**len(d)):
        yield [ _ for _ in range(len(d)) if c & 2**_ ]

def semiperfect(d, n):
    for c in combination(d):
        if sum([ d[_] for _ in c ]) == n:
            return True
    return False

def classify(n):
    d = divisors(n)
    s = sum(d)
    if s == 1:
        return "prime"
    elif s == n:
        return "perfect"
    elif s < n:
        return "deficient"
    elif not semiperfect(d, n):
        return "weird"
    else:
        return "abundant"

def main():
    for i in range(10000):
        print(f"{i} {divisors(i)} is {classify(i)}")

if __name__ == '__main__':
    try:
        main()
    except:
        print("Quit")
