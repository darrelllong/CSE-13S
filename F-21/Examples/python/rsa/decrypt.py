#!/usr/bin/env python3

import argparse, rsa, sys

def int2bytes(n: int) -> bytes:
    return n.to_bytes((n.bit_length() + 7) // 8, "little")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-i", "--infile", nargs="?", type=argparse.FileType("r"),
        default=sys.stdin, help="input file to decrypt."
    )
    parser.add_argument(
        "-o", "--outfile", nargs="?", type=argparse.FileType("wb"),
        default=sys.stdout.buffer, help="output file to write decrypted data"
    )
    parser.add_argument(
        "-k", "--privkey", nargs="?", type=str, default="/tmp/rsa.priv",
        help="path in which the private key is stored."
    )
    args = parser.parse_args()

    with open(args.privkey, "r") as privfile:
        n = int(privfile.readline(), 16)
        d = int(privfile.readline(), 16)

    while line := args.infile.readline():
        c = int(line, 16)
        m = rsa.decrypt(c, d, n)
        args.outfile.write(int2bytes(m)[1:])

if __name__ == "__main__":
    main()
