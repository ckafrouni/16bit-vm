#[repr(u8)]
pub enum OpCode {
    Halt = 0x00,

    MovImmReg = 0x01,
    MovRegReg,
    MovRegMem,
    MovMemReg,

    PushImm = 0x10,
    PushReg,
    PushMem,

    PopReg = 0x20,
    PopMem,
}

// convert u8 to OpCode
impl From<u8> for OpCode {
    fn from(val: u8) -> Self {
        match val {
            0x00 => OpCode::Halt,
            0x01 => OpCode::MovImmReg,
            0x02 => OpCode::MovRegReg,
            0x03 => OpCode::MovRegMem,
            0x04 => OpCode::MovMemReg,
            0x10 => OpCode::PushImm,
            0x11 => OpCode::PushReg,
            0x12 => OpCode::PushMem,
            0x20 => OpCode::PopReg,
            0x21 => OpCode::PopMem,
            _ => panic!("Unknown opcode: {:02x}", val),
        }
    }
}
