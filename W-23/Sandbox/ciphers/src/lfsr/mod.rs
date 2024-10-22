const FEEDBACK: u64 = 0xc000000000000001;

pub struct LFSR {
    word: u64,
}

fn parity(n: u64) -> u64 {
    let mut p = (n >> 32) ^ (n & 0x00000000ffffffff);
    p = (p >> 16) ^ (p & 0x000000000000ffff);
    p = (p >> 8) ^ (p & 0x00000000000000ff);
    p = (p >> 4) ^ (p & 0x000000000000000f);
    p = (p >> 2) ^ (p & 0x0000000000000003);
    (p >> 1) ^ (p & 0x0000000000000001)
}

impl LFSR {
    pub fn new(word: u64) -> Self {
        let mut lfsr = Self { word };
        lfsr.warmup();
        lfsr
    }

    pub fn shift(&mut self) -> u8 {
        let bit = self.word & 1;
        self.word = (self.word >> 1) | (parity(self.word & FEEDBACK) << 63);
        bit as u8
    }

    pub fn warmup(&mut self) {
        for _ in 0..256 {
            self.shift();
        }
    }

    pub fn rand_byte(&mut self) -> u8 {
        (0..8).into_iter().fold(0, |s, _| (s << 1) | self.shift())
    }
}
