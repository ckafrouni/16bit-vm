#[derive(Debug, Clone, Copy)]
#[repr(u8)]
pub enum Reg {
    IP = 0x00,
    SP,
    BP,

    R1 = 0x10,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
}

const NUM_REGS: u32 = 8;

// convert Reg to u8
impl From<Reg> for u8 {
    fn from(val: Reg) -> Self {
        match val {
            Reg::IP => 0x00,
            Reg::SP => 0x01,
            Reg::BP => 0x02,
            Reg::R1 => 0x10,
            Reg::R2 => 0x11,
            Reg::R3 => 0x12,
            Reg::R4 => 0x13,
            Reg::R5 => 0x14,
            Reg::R6 => 0x15,
            Reg::R7 => 0x16,
            Reg::R8 => 0x17,
        }
    }
}

// convert u8 to Reg
impl From<u8> for Reg {
    fn from(val: u8) -> Self {
        match val {
            0x00 => Reg::IP,
            0x01 => Reg::SP,
            0x02 => Reg::BP,
            0x10 => Reg::R1,
            0x11 => Reg::R2,
            0x12 => Reg::R3,
            0x13 => Reg::R4,
            0x14 => Reg::R5,
            0x15 => Reg::R6,
            0x16 => Reg::R7,
            0x17 => Reg::R8,
            _ => panic!("Unknown register: {:02x}", val),
        }
    }
}
