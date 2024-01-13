use super::{Device, DeviceError};

#[derive(Clone)]
pub struct ByteArray {
    data: Vec<u8>,
    start: u32,
}

impl ByteArray {
    pub fn new() -> Self {
        Self {
            data: Vec::new(),
            start: 0,
        }
    }

    pub fn new_with_size(size: u32) -> Self {
        Self {
            data: vec![0; size as usize],
            start: 0,
        }
    }

    pub fn from(data: Vec<u8>, start: u32) -> Self {
        Self { data, start }
    }

    pub fn size(&self) -> u32 {
        self.data.len() as u32
    }

    pub fn push8(&mut self, data: u8) {
        self.data.push(data);
    }

    pub fn push32(&mut self, data: u32) {
        self.data.append(&mut data.to_be_bytes().to_vec());
    }

    pub fn read8(&self, index: u32) -> u8 {
        self.data[index as usize]
    }

    pub fn read32(&self, index: u32) -> u32 {
        u32::from_be_bytes([
            self.data[index as usize],
            self.data[(index + 1) as usize],
            self.data[(index + 2) as usize],
            self.data[(index + 3) as usize],
        ])
    }
}

impl Device for ByteArray {
    fn read8(&self, addr: u32) -> Result<u8, DeviceError> {
        if addr < self.start || addr >= self.start + self.size() {
            return Err(DeviceError::InvalidAddress);
        }

        Ok(self.read8(addr - self.start))
    }

    fn read32(&self, addr: u32) -> Result<u32, DeviceError> {
        if addr < self.start || addr + 3 >= self.start + self.size() {
            return Err(DeviceError::InvalidAddress);
        }

        Ok(self.read32(addr - self.start))
    }

    fn write8(&mut self, addr: u32, value: u8) -> Result<(), DeviceError> {
        if addr < self.start || addr >= self.start + self.size() {
            return Err(DeviceError::InvalidAddress);
        }

        self.data[(addr - self.start) as usize] = value;
        Ok(())
    }

    fn write32(&mut self, addr: u32, value: u32) -> Result<(), DeviceError> {
        if addr < self.start || addr + 3 >= self.start + self.size() {
            return Err(DeviceError::InvalidAddress);
        }

        let bytes = value.to_be_bytes();
        self.data[(addr - self.start) as usize] = bytes[0];
        self.data[(addr - self.start + 1) as usize] = bytes[1];
        self.data[(addr - self.start + 2) as usize] = bytes[2];
        self.data[(addr - self.start + 3) as usize] = bytes[3];
        Ok(())
    }
}

impl std::fmt::Debug for ByteArray {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let sep =
            "\x1B[34m├──────────┼───────────────────────────┼───────────────────────────┼──────────────────┤\x1B[0m";

        let header_line =
            "\x1B[34m┌──────────┬───────────────────────────┬───────────────────────────┬──────────────────┐\x1B[0m";

        let footer_line =
            "\x1B[34m└──────────┴───────────────────────────┴───────────────────────────┴──────────────────┘\x1B[0m";

        let empty_line =
            "\x1B[34m│ ········ │ ·· ·· ·· ·· - ·· ·· ·· ·· │ ·· ·· ·· ·· - ·· ·· ·· ·· │ ················ │\x1B[0m";

        // Header
        writeln!(
            f,
            "{}\n{}\n{}",
            header_line,
            "\x1B[34m│\x1B[0m address  \x1B[34m│\x1B[0m  0  1  2  3 \x1B[2m-\x1B[0m 4  5  6  7  \x1B[34m│\x1B[0m  8  9  a  b \x1B[2m-\x1B[0m c  d  e  f  \x1B[34m│\x1B[0m ascii            \x1B[34m│\x1B[0m",
            sep
        )?;

        let mut prev_empty = false;

        for (i, chunk) in self.data.chunks(16).enumerate() {
            match chunk.iter().position(|&byte| byte != 0) {
                Some(_) => prev_empty = false,
                None => {
                    if prev_empty {
                        continue;
                    } else if i != 0 {
                        prev_empty = true;
                        writeln!(f, "{}", empty_line)?;
                        continue;
                    } else {
                        prev_empty = true;
                    }
                }
            }

            // Address
            write!(
                f,
                "\x1B[34m│\x1B[0m {:08x} \x1B[34m│\x1B[0m ",
                self.start + (i as u32) * 16
            )?;

            // Hexadecimal byte values
            for (j, byte) in chunk.iter().enumerate() {
                write!(
                    f,
                    "{}{}",
                    if *byte != 0 {
                        format!("\x1B[33m{:02x}\x1B[0m", byte)
                    } else {
                        format!("{:02x}", byte)
                    },
                    if j == 7 {
                        " \x1B[34m│\x1B[0m "
                    } else if j == 3 || j == 11 {
                        " \x1B[2m-\x1B[0m "
                    } else {
                        " "
                    }
                )?;
            }
            // Padding for incomplete lines
            for i in chunk.len()..16 {
                write!(
                    f,
                    "  {}",
                    if i == 7 {
                        " \x1B[34m│\x1B[0m "
                    } else if i == 3 || i == 11 {
                        " \x1B[2m-\x1B[0m "
                    } else {
                        " "
                    }
                )?;
            }

            // ASCII representation
            write!(f, "\x1B[34m│\x1B[0m ")?;
            for &byte in chunk {
                write!(
                    f,
                    "{}",
                    if byte.is_ascii_graphic() || byte == b' ' {
                        format!("\x1B[34m{}\x1B[0m", byte as char)
                    } else {
                        format!("\x1B[2m{}\x1B[0m", '.')
                    }
                )?;
            }
            // Padding for incomplete lines
            for _ in chunk.len()..16 {
                write!(f, " ")?;
            }

            writeln!(f, " \x1B[34m│\x1B[0m")?;
        }

        // Footer
        writeln!(f, "{}", footer_line)
    }
}
