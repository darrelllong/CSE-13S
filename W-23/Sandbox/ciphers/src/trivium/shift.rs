pub struct ShiftRegister {
    word: u128,
    length: usize,
}

impl ShiftRegister {
    pub fn new(bits: usize) -> Self {
        Self {
            word: 0,
            length: std::cmp::min(bits, 128),
        }
    }

    pub fn from_word(bits: usize, word: u128) -> Self {
        let mut sr = Self::new(bits);
        sr.word = word;
        sr
    }

    pub fn get(&self, pos: usize) -> u8 {
        ((self.word >> pos) & 1) as u8
    }

    pub fn set(&mut self, pos: usize) {
        self.word |= 1 << pos
    }

    pub fn shift(&mut self, bit: u8) -> u8 {
        let out = self.get(self.length - 1);
        self.word = self.word << 1 | bit as u128;
        out
    }
}

impl std::fmt::Debug for ShiftRegister {
    fn fmt(&self, fmt: &mut std::fmt::Formatter) -> std::fmt::Result {
        let masked = self.word & ((1 << self.length) - 1);
        fmt.debug_struct("ShiftRegister")
            .field("length", &self.length)
            .field("word", &format_args!("{:01$b}", &masked, self.length))
            .finish()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn shift() {
        let mut sr = ShiftRegister::from_word(3, 0b11);
        assert_eq!(0, sr.shift(0));
        assert_eq!(1, sr.shift(0));
        assert_eq!(1, sr.shift(0));
        assert_eq!(0, sr.shift(0));

        let mut sr = ShiftRegister::from_word(1, 0b1);
        assert_eq!(1, sr.shift(0));
        assert_eq!(0, sr.shift(0));

        let mut sr = ShiftRegister::from_word(9, 0b100110011);
        assert_eq!(1, sr.shift(0));
        assert_eq!(0, sr.shift(0));
        assert_eq!(0, sr.shift(0));
        assert_eq!(1, sr.shift(0));
        assert_eq!(1, sr.shift(0));
        assert_eq!(0, sr.shift(0));
        assert_eq!(0, sr.shift(0));
        assert_eq!(1, sr.shift(0));
        assert_eq!(1, sr.shift(0));
        assert_eq!(0, sr.shift(0));
    }
}
