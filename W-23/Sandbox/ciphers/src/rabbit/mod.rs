//! [Original paper and C implementation]
//!  - web.archive.org/web/20131211141149/http://www.cryptico.com/images/pages/rabbit_fse.pdf
//! [Test vectors]
//!  - rfc-editor.org/rfc/rfc4503#page-8

use std::collections::VecDeque;

#[derive(Debug)]
pub struct Rabbit {
    carry: u32,
    x: [u32; 8],
    c: [u32; 8],
    buffer: VecDeque<u8>,
}

impl Rabbit {
    /// Create a new `Rabbit` instance with a specified key.
    pub fn new(key: u128) -> Self {
        let mut rabbit = Self {
            carry: 0,
            x: [0u32; 8],
            c: [0u32; 8],
            buffer: VecDeque::with_capacity(16),
        };
        rabbit.setup(&key.to_le_bytes());
        rabbit
    }

    /// Square a 32-bit number to obtain the 64-bit result.
    /// Return the upper 32 bits XOR the lower 32 bits.
    fn g_func(x: u32) -> u32 {
        // Construct high and low argument for squaring.
        let a = x & 0x0000FFFF;
        let b = x >> 16;

        // Calculate high and low result of squaring.
        let h = ((((a * a) >> 17) + (a * b)) >> 15) + b * b;
        let l = x * x;

        // Return high XOR low.
        h ^ l
    }

    /// Return 1 if x is less than y else 0.
    fn cmp(x: u32, y: u32) -> u32 {
        if x < y {
            1
        } else {
            0
        }
    }

    /// Calculate the next internal state.
    fn next_state(&mut self) {
        // Temporary data.
        let c_old = self.c;
        let mut g = [0u32; 8];

        // Calculate new counter values.
        self.c[0] += 0x4d34d34d + self.carry;
        self.c[1] += 0xd34d34d3 + Self::cmp(self.c[0], c_old[0]);
        self.c[2] += 0x34d34d34 + Self::cmp(self.c[1], c_old[1]);
        self.c[3] += 0x4d34d34d + Self::cmp(self.c[2], c_old[2]);
        self.c[4] += 0xd34d34d3 + Self::cmp(self.c[3], c_old[3]);
        self.c[5] += 0x34d34d34 + Self::cmp(self.c[4], c_old[4]);
        self.c[6] += 0x4d34d34d + Self::cmp(self.c[5], c_old[5]);
        self.c[7] += 0xd34d34d3 + Self::cmp(self.c[6], c_old[6]);
        self.carry = Self::cmp(self.c[7], c_old[7]);

        // Calculate the g-functions.
        for i in 0..8 {
            g[i] = Self::g_func(self.x[i] + self.c[i]);
        }

        // Calculate new state values.
        self.x[0] = g[0] + g[7].rotate_left(16) + g[6].rotate_left(16);
        self.x[1] = g[1] + g[0].rotate_left(8) + g[7];
        self.x[2] = g[2] + g[1].rotate_left(16) + g[0].rotate_left(16);
        self.x[3] = g[3] + g[2].rotate_left(8) + g[1];
        self.x[4] = g[4] + g[3].rotate_left(16) + g[2].rotate_left(16);
        self.x[5] = g[5] + g[4].rotate_left(8) + g[3];
        self.x[6] = g[6] + g[5].rotate_left(16) + g[4].rotate_left(16);
        self.x[7] = g[7] + g[6].rotate_left(8) + g[5];
    }

    /// Key setup.
    fn setup(&mut self, key: &[u8]) {
        // Generate four subkeys.
        let k = unsafe { std::mem::transmute::<&[u8], &[u32]>(key) };

        // Generate initial state variables.
        self.x[0] = k[0];
        self.x[2] = k[1];
        self.x[4] = k[2];
        self.x[6] = k[3];
        self.x[1] = (k[3] << 16) | (k[2] >> 16);
        self.x[3] = (k[0] << 16) | (k[3] >> 16);
        self.x[5] = (k[1] << 16) | (k[0] >> 16);
        self.x[7] = (k[2] << 16) | (k[1] >> 16);

        // Generate initial counter values.
        self.c[0] = k[2].rotate_left(16);
        self.c[2] = k[3].rotate_left(16);
        self.c[4] = k[0].rotate_left(16);
        self.c[6] = k[1].rotate_left(16);
        self.c[1] = (k[0] & 0xFFFF0000) | (k[1] & 0x0000FFFF);
        self.c[3] = (k[1] & 0xFFFF0000) | (k[2] & 0x0000FFFF);
        self.c[5] = (k[2] & 0xFFFF0000) | (k[3] & 0x0000FFFF);
        self.c[7] = (k[3] & 0xFFFF0000) | (k[0] & 0x0000FFFF);

        // Reset carry flag.
        self.carry = 0;

        // Iterate the system four times.
        for _ in 0..4 {
            self.next_state();
        }

        // Modify the counters.
        for i in 0..8 {
            self.c[(i + 4) & 0x7] ^= self.x[i];
        }
    }

    /// Iterate the system and produce the next key block.
    pub fn next_block(&mut self) -> [u32; 4] {
        self.next_state();
        let k0 = self.x[0] ^ (self.x[5] >> 16) ^ (self.x[3] << 16);
        let k1 = self.x[2] ^ (self.x[7] >> 16) ^ (self.x[5] << 16);
        let k2 = self.x[4] ^ (self.x[1] >> 16) ^ (self.x[7] << 16);
        let k3 = self.x[6] ^ (self.x[3] >> 16) ^ (self.x[1] << 16);
        [k0, k1, k2, k3]
    }

    /// Encrypt or decrypt a block of data.
    pub fn cipher(&mut self, src: &mut [u8], dst: &mut [u8]) {
        for i in (0..src.len()).step_by(16) {
            // Interpret bytes as words.
            let src = unsafe { std::mem::transmute::<&mut [u8], &mut [u32]>(&mut src[i..]) };
            let dst = unsafe { std::mem::transmute::<&mut [u8], &mut [u32]>(&mut dst[i..]) };

            // Encrypt a block of data.
            let k = self.next_block();
            dst[0] = src[0] ^ k[0];
            dst[1] = src[1] ^ k[1];
            dst[2] = src[2] ^ k[2];
            dst[3] = src[3] ^ k[3];
        }
    }

    /// Produce a random byte.
    pub fn rand_byte(&mut self) -> u8 {
        // Fill buffer if empty.
        if self.buffer.is_empty() {
            for k in self.next_block() {
                self.buffer.extend(k.to_le_bytes());
            }
        }

        // We will always have another byte.
        self.buffer.pop_front().unwrap()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_vectors() {
        let tests = [
            (
                0x00000000000000000000000000000000,
                (0xb15754f036a5d6ecf56b45261c4af702u128).to_le_bytes(),
                (0x88e8d815c59c0c397b696c4789c68aa7u128).to_le_bytes(),
                (0xF416A1C3700CD451DA68D1881673D696u128).to_le_bytes(),
            ),
            (
                0x912813292e3d36fe3bfc62f1dc51c3ac,
                (0x3d2df3c83ef627a1e97fc38487e2519cu128).to_le_bytes(),
                (0xf576cd61f4405b8896bf53aa8554fc19u128).to_le_bytes(),
                (0xe5547473fbdb43508ae53b20204d4c5eu128).to_le_bytes(),
            ),
            (
                0x8395741587e0c733e9e9ab01c09b0043,
                (0x0cb10dcda041cdac32eb5cfd02d0609bu128).to_le_bytes(),
                (0x95fc9fca0f17015a7b7092114cff3eadu128).to_le_bytes(),
                (0x9649e5de8bfc7f3f924147ad3a947428u128).to_le_bytes(),
            ),
        ];

        for (key, s0, s1, s2) in tests {
            let mut rabbit = Rabbit::new(key);
            let (mut src, mut dst) = ([0u8; 16], [0u8; 16]);
            rabbit.cipher(&mut src, &mut dst);
            assert_eq!(s0, dst);
            rabbit.cipher(&mut src, &mut dst);
            assert_eq!(s1, dst);
            rabbit.cipher(&mut src, &mut dst);
            assert_eq!(s2, dst);
        }
    }
}
