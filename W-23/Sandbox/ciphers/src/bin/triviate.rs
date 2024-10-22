use anyhow::Result;
use byteorder::{ReadBytesExt, WriteBytesExt};
use ciphers::Trivium;
use std::io::{self, BufReader, BufWriter};

fn main() -> Result<()> {
    let mut reader = BufReader::new(io::stdin());
    let mut writer = BufWriter::new(io::stdout());

    let key = 0xd642738d7e95158dcc42211804b3d194;
    let nonce = 0x8f0fb334746e1c0de8e420f3b4ae270e;
    let mut cipher = Trivium::new(key, nonce);

    while let Ok(byte) = reader.read_u8() {
        writer.write_u8(byte ^ cipher.rand_byte())?;
    }

    Ok(())
}
