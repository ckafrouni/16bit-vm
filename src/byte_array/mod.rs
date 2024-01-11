#[derive(Clone)]
pub struct ByteArray {
    data: Vec<u8>,
    start: u32,
    size: u32,
}

impl ByteArray {
    pub fn new() -> Self {
        Self {
            data: Vec::new(),
            start: 0,
            size: 0,
        }
    }

    pub fn from(data: Vec<u8>, start: u32) -> Self {
        let size = data.len() as u32;
        Self { data, start, size }
    }

    pub fn size(&self) -> u32 {
        self.size
    }

    pub fn push8(&mut self, data: u8) {
        self.data.push(data);
        self.size += 1;
    }

    pub fn push32(&mut self, data: u32) {
        self.data.append(&mut data.to_be_bytes().to_vec());
        self.size += 4;
    }

    pub fn get8(&self, index: u32) -> u8 {
        self.data[index as usize]
    }

    pub fn get32(&self, index: u32) -> u32 {
        u32::from_be_bytes([
            self.data[index as usize],
            self.data[(index + 1) as usize],
            self.data[(index + 2) as usize],
            self.data[(index + 3) as usize],
        ])
    }

    pub fn slice(&self, start: u32, end: u32) -> ByteArray {
        ByteArray::from(self.data[start as usize..end as usize].to_vec(), start)
    }
}

impl std::fmt::Debug for ByteArray {
    /**
     * │ address  │  0  1  2  3   4  5  6  7 │  8  9  a  b   c  d  e  f │ ascii
     * ───────────┼──────────────────────────┼──────────────────────────┼─────────────────
     * │ 00000000 │ 01 02 03 04  05 06 07 08 │ 09 0a 0b 0c  0d 0e 0f 10 │ ................
     * │ 00000010 │ 11 12 13 14  15 16 17 18 │ 19 1a 1b 1c  1d 1e 1f 20 │ ................
     * │ 00000020 │ 21 22 23 24  25 26 27 28 │ 29 2a 2b 2c  2d 2e 2f 30 │ !"#$%&'()*+,-./0
     * │ 00000030 │ 00 00 00 00  00 00 00 00 │ 00 00 00 00  00 00 00 00 │ ................
     * ───────────┼──────────────────────────┼──────────────────────────┼─────────────────
     * │ 00000090 │ 00 00 12 34  56 78 9a bc │ de f0 00 00  00 00 00 00 │ ...4Vx..........
     * │ 000000a0 │ 00 00 00 00  00 00 00 00 │ 00 00 00 00  00 00 00 00 │ ................
     */
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let sep =
            "\x1B[34m├──────────┼───────────────────────────┼───────────────────────────┼──────────────────┤\x1B[0m";

        let header_line =
            "\x1B[34m┌──────────┬───────────────────────────┬───────────────────────────┬──────────────────┐\x1B[0m";

        let footer_line =
            "\x1B[34m└──────────┴───────────────────────────┴───────────────────────────┴──────────────────┘\x1B[0m";

        // Header
        writeln!(
            f,
            "{}\n{}\n{}",
            header_line,
            "\x1B[34m│\x1B[0m address  \x1B[34m│\x1B[0m  0  1  2  3 \x1B[2m-\x1B[0m 4  5  6  7  \x1B[34m│\x1B[0m  8  9  a  b \x1B[2m-\x1B[0m c  d  e  f  \x1B[34m│\x1B[0m ascii            \x1B[34m│\x1B[0m",
            sep
        )?;

        for (i, chunk) in self.data.chunks(16).enumerate() {
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
