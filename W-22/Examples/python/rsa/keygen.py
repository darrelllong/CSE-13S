#!/usr/bin/env python3

import argparse, rsa

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-b", "--bits", nargs="?", type=int, default=2048,
        help="bits in the key to create."
    )
    parser.add_argument(
        "-e", "--pubkey", nargs="?", type=str, default="/tmp/rsa.pub",
        help="path in which to store the public key."
    )
    parser.add_argument(
        "-d", "--privkey", nargs="?", type=str, default="/tmp/rsa.priv",
        help="path in which to store the private key."
    )
    args = parser.parse_args()

    (e, d, n) = rsa.makeKey(args.bits)

    # Public key: (n, e)
    with open(args.pubkey, "w") as pubfile:
        print(hex(n), file=pubfile)
        print(hex(e), file=pubfile)

    # Private key: (n, d)
    with open(args.privkey, "w") as privkey:
        print(hex(n), file=privkey)
        print(hex(d), file=privkey)

if __name__ == "__main__":
    main()
