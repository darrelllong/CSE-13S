mod shift;

use shift::ShiftRegister;

const MASK: u128 = 0x000000000000ffffffffffffffffffff;

#[derive(Debug)]
pub struct Trivium {
    a: ShiftRegister,
    b: ShiftRegister,
    c: ShiftRegister,
}

impl Trivium {
    fn clock(&mut self) -> u8 {
        let a = self.a.get(65) ^ self.a.get(92) ^ (self.a.get(90) & self.a.get(91));
        let b = self.b.get(68) ^ self.b.get(83) ^ (self.b.get(81) & self.a.get(82));
        let c = self.c.get(65) ^ self.c.get(110) ^ (self.c.get(108) & self.a.get(109));
        self.a.shift(self.a.get(86) ^ c);
        self.b.shift(self.b.get(77) ^ a);
        self.c.shift(self.c.get(86) ^ b);
        a ^ b ^ c
    }

    fn warmup(&mut self) {
        for _ in 0..1152 {
            self.clock();
        }
    }

    pub fn new(key: u128, nonce: u128) -> Self {
        let mut cipher = Self {
            a: ShiftRegister::from_word(93, key & MASK),
            b: ShiftRegister::from_word(84, nonce & MASK),
            c: ShiftRegister::new(111),
        };

        cipher.c.set(108);
        cipher.c.set(109);
        cipher.c.set(110);

        cipher.warmup();
        cipher
    }

    pub fn rand_byte(&mut self) -> u8 {
        (0..8).into_iter().fold(0, |s, _| (s << 1) | self.clock())
    }
}
