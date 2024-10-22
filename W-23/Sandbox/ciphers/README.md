# ciphers

Contains the Rabbit cipher, Trivium cipher, and implementations of
linear-feedback shift registers (LFSRs) and feedback with carry shift registers
(FCSRs).

Four encryption/decryption programs are bundled in this package:
1. `carryover` - Encryption using FCSRs.
2. `feed` - Encryption using LFSRs.
3. `rabbitize` - Encryption using Rabbit.
4. `trivate` - Encryption using Trivium.

## Building

```bash
$ cargo build --release
```

## Running

```bash
$ cat <file> | <program> | <program> | diff - <file>
```

## Cleaning

```bash
$ cargo clean
```
