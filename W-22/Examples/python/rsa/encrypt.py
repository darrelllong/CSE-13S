#!/usr/bin/env python3

from math import floor, log2
import argparse, rsa, sys

def bytes2int(b: bytes) -> int:
    return int.from_bytes(b, "little")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-i", "--infile", nargs="?", type=argparse.FileType("rb"),
        default=sys.stdin.buffer, help="input file to encrypt."
    )
    parser.add_argument(
        "-o", "--outfile", nargs="?", type=argparse.FileType("w"),
        default=sys.stdout, help="outfile file to write encrypted data."
    )
    parser.add_argument(
        "-k", "--pubkey", nargs="?", type=str, default="/tmp/rsa.pub",
        help="path in which the public key is stored."
    )
    args = parser.parse_args()

    with open(args.pubkey, "r") as pubfile:
        n = int(pubfile.readline(), 16)
        e = int(pubfile.readline(), 16)

    block = (floor(log2(n)) - 1) // 8
    while data := args.infile.read(block - 1):
        m = bytes2int(bytes([0xff]) + data)
        c = rsa.encrypt(m, e, n)
        print(hex(c), file=args.outfile)

if __name__ == "__main__":
    main()
