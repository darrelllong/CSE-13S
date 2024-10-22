def even(n): return n % 2 == 0

def odd(n): return n % 2 == 1

def powerMod(a, d, n):
    v = 1
    p = a
    while d > 0:
        if odd(d):
            v = (v * p) % n
        p = (p * p) % n
        d = d // 2
    return v

from random import randrange as uniform

def witness(a, n):
    u = n - 1
    t = 0
    while even(u):
        t = t + 1
        u = u // 2
    x = powerMod(a, u, n)
    for _ in range(0, t):
        y = powerMod(x, 2, n)
        if y == 1 and x != 1 and x != n - 1:
            return True
        x = y
    return x != 1

def isPrime(n, k):
    if n < 2 or (n != 2 and n % 2 == 0):
        return False
    if n < 4:
        return True
    for _ in range (0, k):
        a = uniform(2, n)
        if witness(a, n):
            return False
    return True

def gcd(a, b):
    if b == 0:
        return a
    else:
        return gcd(b, a % b)

def lcm(a, b):
    return abs(a * b) // gcd(a, b)

def inverse(a, n):
    r, rP = n, a
    t, tP = 0, 1
    while rP != 0:
        q = r // rP
        r, rP = rP, r - q * rP
        t, tP = tP, t - q * tP
    if r > 1:
        return "no inverse"
    if t < 0:
        return t + n
    else:
        return t

def randomPrime(low, high):
    guess = 0 # Certainly not prime!
    while not isPrime(guess, 100):
        guess = uniform(low, high) # Half will be even, the rest have Pr[prime] ≈ 1/log(N).
    return guess

def makeKey(bits):
    size = bits // 2
    low  = 2**(size - 1) # Assure the primes are each approximately half of the
    high = 2**size - 1   # bits in the modulus.
    p = randomPrime(low, high)
    q = randomPrime(low, high)
    𝝺 = lcm(p - 1, q - 1) # Carmichael 𝝺(n) = lcm(𝝺(p), 𝝺(q)) = lcm(p - 1, q - 1)
    e = 2**16 + 1         # Default public exponent
    while gcd(e, 𝝺) != 1: # Happens only if we are very unlucky
        e = randomPrime(low, high)
    d = inverse(e, 𝝺) # The private key
    n = p * q         # The modulus
    return (e, d, n)

def encode(s):
    sum = 0
    pow = 1
    for c in s:
        sum += pow * ord(c)
        pow *= 256
    return sum

def decode(n):
    s = ""
    while n > 0:
        s = s + chr(n % 256)
        n //= 256
    return s

def encrypt(m, e, n): return powerMod(m, e, n)

def decrypt(c, d, n): return powerMod(c, d, n)
