use anyhow::Result;
use byteorder::{ReadBytesExt, WriteBytesExt};
use ciphers::Rabbit;
use std::io::{self, BufReader, BufWriter};

fn main() -> Result<()> {
    let mut reader = BufReader::new(io::stdin());
    let mut writer = BufWriter::new(io::stdout());

    let key = 0x8395741587e0c733e9e9ab01c09b0043;
    let mut cipher = Rabbit::new(key);

    while let Ok(byte) = reader.read_u8() {
        writer.write_u8(byte ^ cipher.rand_byte())?;
    }

    Ok(())
}
