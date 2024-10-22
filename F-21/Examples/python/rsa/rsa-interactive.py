#!/usr/bin/env python3

import rsa

def main():
    try:
        bits = int(input("How many bits? "))
    except:
        print("We needed a positive integer!")
        quit()

    (e, d, n) = rsa.makeKey(bits)

    print(f"{e = }")
    print(f"{d = }")
    print(f"{n = }")

    m = ""
    try:
        while m != "quit":
            m = input(">> ")
            c = rsa.encrypt(m, e, n); print(f"En[{m}] = {c}")
            t = rsa.decrypt(c, d, n); print(f"De[{c}] = {t}")
    except:
        print("So long!")

if __name__ == "__main__":
    main()
