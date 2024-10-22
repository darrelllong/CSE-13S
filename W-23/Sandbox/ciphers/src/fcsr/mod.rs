const TAP_SEQUENCE: [u64; 4] = [63, 13, 2, 1];

pub struct FCSR {
    state: u64,
    carry: u64,
}

impl FCSR {
    pub fn new(state: u64) -> Self {
        Self { state, carry: 0 }
    }

    pub fn sum(&self) -> u64 {
        TAP_SEQUENCE
            .iter()
            .fold(0, |s, i| s + ((self.state >> i) & 1))
    }

    pub fn shift(&mut self) -> u8 {
        let output = self.state & 1;
        let feedback = self.sum() + self.carry;
        let input = feedback & 1;
        self.carry = feedback / 2;
        self.state = (self.state >> 1) | (input << 63);
        output as u8
    }

    pub fn rand_byte(&mut self) -> u8 {
        (0..8).into_iter().fold(0, |s, _| (s << 1) | self.shift())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn taps() {
        let state = 0x8000000000002006;
        let fcsr = FCSR::new(state);
        assert_eq!(fcsr.sum(), 4);
    }
}
