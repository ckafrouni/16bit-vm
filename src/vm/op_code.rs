use super::reg::Reg;

#[derive(Debug, Clone, Copy)]
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

impl OpCode {
    #[allow(dead_code)]
    pub fn operands(&self) -> Vec<Operand> {
        match self {
            OpCode::Halt => vec![],
            OpCode::MovImmReg => vec![Operand::Imm(0), Operand::Reg(Reg::R1)],
            OpCode::MovRegReg => vec![Operand::Reg(Reg::R1), Operand::Reg(Reg::R2)],
            OpCode::MovRegMem => vec![Operand::Reg(Reg::R1), Operand::Mem(0)],
            OpCode::MovMemReg => vec![Operand::Mem(0), Operand::Reg(Reg::R1)],
            OpCode::PushImm => vec![Operand::Imm(0)],
            OpCode::PushReg => vec![Operand::Reg(Reg::R1)],
            OpCode::PushMem => vec![Operand::Mem(0)],
            OpCode::PopReg => vec![Operand::Reg(Reg::R1)],
            OpCode::PopMem => vec![Operand::Mem(0)],
        }
    }

    pub fn size(&self) -> u32 {
        match self {
            OpCode::Halt => 1,
            OpCode::MovImmReg => 1 + 4 + 1,
            OpCode::MovRegReg => 1 + 1 + 1,
            OpCode::MovRegMem => 1 + 1 + 4,
            OpCode::MovMemReg => 1 + 4 + 1,
            OpCode::PushImm => 1 + 4,
            OpCode::PushReg => 1 + 1,
            OpCode::PushMem => 1 + 4,
            OpCode::PopReg => 1 + 1,
            OpCode::PopMem => 1 + 4,
        }
    }
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

#[derive(Clone, Copy)]
pub enum Operand {
    None,
    Imm(u32),
    Reg(Reg),
    Mem(u32),
}

impl Operand {
    #[allow(dead_code)]
    pub fn size(&self) -> u32 {
        match self {
            Operand::None => 0,
            Operand::Imm(_) => 4,
            Operand::Reg(_) => 1,
            Operand::Mem(_) => 4,
        }
    }

    pub fn unwrap_imm(&self) -> u32 {
        match self {
            Operand::Imm(val) => *val,
            _ => panic!("Operand is not an immediate value"),
        }
    }

    pub fn unwrap_reg(&self) -> Reg {
        match self {
            Operand::Reg(val) => *val,
            _ => panic!("Operand is not a register"),
        }
    }

    pub fn unwrap_mem(&self) -> u32 {
        match self {
            Operand::Mem(val) => *val,
            _ => panic!("Operand is not a memory address"),
        }
    }
}

impl std::fmt::Debug for Operand {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Operand::None => write!(f, "None"),
            Operand::Imm(val) => write!(f, "$0x{:08x}", val),
            Operand::Reg(val) => write!(f, "{:?}", val),
            Operand::Mem(val) => write!(f, "&0x{:08x}", val),
        }
    }
}
