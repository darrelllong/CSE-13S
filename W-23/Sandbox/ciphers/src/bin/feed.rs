use anyhow::Result;
use byteorder::{ReadBytesExt, WriteBytesExt};
use ciphers::LFSR;
use std::io::{self, BufReader, BufWriter};

fn main() -> Result<()> {
    let mut reader = BufReader::new(io::stdin());
    let mut writer = BufWriter::new(io::stdout());

    let mut counter = 0;
    let key = (0x8badbeefu64).to_le_bytes();
    let nonce = 0x614472726c65216c;
    let mut cipher = LFSR::new(nonce);

    while let Ok(byte) = reader.read_u8() {
        writer.write_u8(key[counter] ^ byte ^ cipher.rand_byte())?;
        counter = (counter + 1) % key.len();
    }

    Ok(())
}
