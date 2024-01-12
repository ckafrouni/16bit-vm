use std::io::Write;

use super::{
    mmu::MMU,
    op_code::{OpCode, Operand},
    reg::Reg,
    VMError,
};

#[derive(Clone, Copy, PartialEq, Debug)]
pub enum CPUState {
    Running,
    Halted,
}

#[allow(unused)]
pub enum CPUMode {
    Debug,            // Debug mode : will print debug info
    DebugInteractive, // Debug mode : will print debug info and wait for user input
    Release,
}

#[derive(Debug, Clone)]
pub enum CPUError {
    CPUNotRunning,
}

pub struct CPU {
    state: CPUState,
    mmu: MMU,

    // Special purpose registers
    ip: u32,
    sp: u32,
    bp: u32,

    // General purpose registers
    r1: u32,
    r2: u32,
    r3: u32,
    r4: u32,
    r5: u32,
    r6: u32,
    r7: u32,
    r8: u32,

    modified_regs: Vec<Reg>,
}

// Contains methods for reading and writing registers
// and memory, along with other attributes
impl CPU {
    pub fn new(mmu: MMU) -> Self {
        Self {
            state: CPUState::Halted,
            modified_regs: Vec::new(),
            mmu,
            ip: 0,
            sp: 0,
            bp: 0,
            r1: 0,
            r2: 0,
            r3: 0,
            r4: 0,
            r5: 0,
            r6: 0,
            r7: 0,
            r8: 0,
        }
    }

    pub fn set_reg(&mut self, reg: Reg, val: u32) {
        match reg {
            Reg::IP => self.ip = val,
            Reg::SP => self.sp = val,
            Reg::BP => self.bp = val,
            Reg::R1 => self.r1 = val,
            Reg::R2 => self.r2 = val,
            Reg::R3 => self.r3 = val,
            Reg::R4 => self.r4 = val,
            Reg::R5 => self.r5 = val,
            Reg::R6 => self.r6 = val,
            Reg::R7 => self.r7 = val,
            Reg::R8 => self.r8 = val,
        }
    }

    pub fn get_reg(&self, reg: Reg) -> u32 {
        match reg {
            Reg::IP => self.ip,
            Reg::SP => self.sp,
            Reg::BP => self.bp,
            Reg::R1 => self.r1,
            Reg::R2 => self.r2,
            Reg::R3 => self.r3,
            Reg::R4 => self.r4,
            Reg::R5 => self.r5,
            Reg::R6 => self.r6,
            Reg::R7 => self.r7,
            Reg::R8 => self.r8,
        }
    }

    pub fn set_mem(&mut self, addr: u32, val: u32) {
        self.mmu.write32(addr, val).unwrap();
    }

    pub fn get_mem(&self, addr: u32) -> u32 {
        self.mmu.read32(addr).unwrap()
    }

    pub fn slice(&self, start: u32, end: u32) -> Result<Vec<u8>, VMError> {
        self.mmu.slice(start, end).map_err(|err| err.into())
    }
}

// Contains methods for stepping through the code
impl CPU {
    pub fn run(&mut self, mode: CPUMode) -> Result<u32, VMError> {
        match mode {
            CPUMode::Debug => self.run_debug(),
            CPUMode::DebugInteractive => self.run_debug_interactive(),
            CPUMode::Release => self.run_release(),
        }
    }

    pub fn run_debug(&mut self) -> Result<u32, VMError> {
        let mut _n_ops = 0;
        self.state = CPUState::Running;

        println!("Starting CPU state:");
        println!("{:?}", self);

        while self.state == CPUState::Running {
            self.modified_regs.clear();
            _n_ops += 1;
            let (op, operands) = self.fetch()?;
            println!(
                "Step {:02}\nIP: {:08x} - {:?} {:?}",
                _n_ops, self.ip, op, operands
            );
            self.execute(op, operands);
            println!("{:?}", self);
        }
        Ok(self.r1)
    }

    pub fn run_debug_interactive(&mut self) -> Result<u32, VMError> {
        let mut _n_ops = 0;
        self.state = CPUState::Running;

        println!("Starting CPU state:");
        println!("{:?}", self);

        while self.state == CPUState::Running {
            self.modified_regs.clear();
            _n_ops += 1;

            let (op, operands) = self.fetch()?;
            println!(
                "Step {:02}\nIP: {:08x} - {:?} {:?}",
                _n_ops, self.ip, op, operands
            );

            print!("\x1b[33;2m<enter> to continue... \x1b[0m");
            std::io::stdout().flush().unwrap(); // Flush the print statement

            let mut _input = String::new();
            std::io::stdin()
                .read_line(&mut _input)
                .expect("Failed to read line");

            self.execute(op, operands);

            println!("{:?}", self);
        }

        println!("Stack:");
        println!("{:?}", self.mmu);

        Ok(self.r1)
    }

    pub fn run_release(&mut self) -> Result<u32, VMError> {
        self.state = CPUState::Running;
        while self.state == CPUState::Running {
            let (op, operands) = self.fetch()?;
            self.execute(op, operands);
        }
        Ok(self.r1)
    }
}

impl CPU {
    pub fn fetch(&mut self) -> Result<(OpCode, Vec<Operand>), VMError> {
        if self.state != CPUState::Running {
            return Err(CPUError::CPUNotRunning.into());
        }

        let mut ip = self.ip;

        let op = OpCode::from(self.mmu.read8(ip)?);
        ip += 1;

        match op {
            OpCode::Halt => Ok((OpCode::Halt, [Operand::None].to_vec())),

            OpCode::MovImmReg => {
                let imm = self.mmu.read32(ip)?;
                ip += 4;

                let dst_reg: Reg = Reg::from(self.mmu.read8(ip)?);

                Ok((
                    OpCode::MovImmReg,
                    [Operand::Imm(imm), Operand::Reg(dst_reg)].to_vec(),
                ))
            }

            OpCode::MovRegReg => {
                let src_reg = Reg::from(self.mmu.read8(ip)?);
                ip += 1;

                let dst_reg = Reg::from(self.mmu.read8(ip)?);

                Ok((
                    OpCode::MovRegReg,
                    [Operand::Reg(src_reg), Operand::Reg(dst_reg)].to_vec(),
                ))
            }

            OpCode::MovRegMem => {
                let src_reg = Reg::from(self.mmu.read8(ip)?);
                ip += 1;

                let dst_addr: u32 = self.mmu.read32(ip)?;

                Ok((
                    OpCode::MovRegMem,
                    [Operand::Reg(src_reg), Operand::Mem(dst_addr)].to_vec(),
                ))
            }

            OpCode::MovMemReg => {
                let src_addr = self.mmu.read32(ip)?;
                ip += 4;

                let dst_reg = Reg::from(self.mmu.read8(ip)?);

                Ok((
                    OpCode::MovMemReg,
                    [Operand::Mem(src_addr), Operand::Reg(dst_reg)].to_vec(),
                ))
            }

            OpCode::PushImm => {
                let imm = self.mmu.read32(ip)?;

                Ok((OpCode::PushImm, [Operand::Imm(imm)].to_vec()))
            }

            OpCode::PushReg => {
                let reg = Reg::from(self.mmu.read8(ip)?);

                Ok((OpCode::PushReg, [Operand::Reg(reg)].to_vec()))
            }

            OpCode::PushMem => {
                let addr = self.mmu.read32(ip)?;

                Ok((OpCode::PushMem, [Operand::Mem(addr)].to_vec()))
            }

            OpCode::PopReg => {
                let reg = Reg::from(self.mmu.read8(ip)?);

                Ok((OpCode::PopReg, [Operand::Reg(reg)].to_vec()))
            }

            OpCode::PopMem => {
                let addr = self.mmu.read32(ip)?;

                Ok((OpCode::PopMem, [Operand::Mem(addr)].to_vec()))
            }
        }
    }
}

impl CPU {
    fn execute(&mut self, op: OpCode, operands: Vec<Operand>) {
        match op {
            OpCode::Halt => self.halt(),

            OpCode::MovImmReg => {
                let imm = operands[0].unwrap_imm();
                let dst_reg = operands[1].unwrap_reg();
                self.mov_imm_reg(imm, dst_reg);
            }

            OpCode::MovRegReg => {
                let src_reg = operands[0].unwrap_reg();
                let dst_reg = operands[1].unwrap_reg();
                self.mov_reg_reg(src_reg, dst_reg);
            }

            OpCode::MovRegMem => {
                let src_reg = operands[0].unwrap_reg();
                let dst_addr = operands[1].unwrap_mem();
                self.mov_reg_mem(src_reg, dst_addr);
            }

            OpCode::MovMemReg => {
                let src_addr = operands[0].unwrap_mem();
                let dst_reg = operands[1].unwrap_reg();
                self.mov_mem_reg(src_addr, dst_reg);
            }

            OpCode::PushImm => {
                let imm = operands[0].unwrap_imm();
                self.push_imm(imm);
            }

            OpCode::PushReg => {
                let reg = operands[0].unwrap_reg();
                self.push_reg(reg);
            }

            OpCode::PushMem => {
                let addr = operands[0].unwrap_mem();
                self.push_mem(addr);
            }

            OpCode::PopReg => {
                let reg = operands[0].unwrap_reg();
                self.pop_reg(reg);
            }

            OpCode::PopMem => {
                let addr = operands[0].unwrap_mem();
                self.pop_mem(addr);
            }
        }
    }

    fn halt(&mut self) {
        self.state = CPUState::Halted;
        self.ip += OpCode::Halt.size();
    }

    fn mov_imm_reg(&mut self, imm: u32, dst_reg: Reg) {
        self.set_reg(dst_reg, imm);
        self.ip += OpCode::MovImmReg.size();
        self.modified_regs.push(dst_reg);
        self.modified_regs.push(Reg::IP);
    }

    fn mov_reg_reg(&mut self, src_reg: Reg, dst_reg: Reg) {
        let val = self.get_reg(src_reg);
        self.set_reg(dst_reg, val);
        self.ip += OpCode::MovRegReg.size();
        self.modified_regs.push(dst_reg);
        self.modified_regs.push(Reg::IP);
    }

    fn mov_reg_mem(&mut self, src_reg: Reg, dst: u32) {
        let val = self.get_reg(src_reg);
        self.set_mem(dst, val);
        self.ip += OpCode::MovRegMem.size();
        self.modified_regs.push(Reg::IP);
    }

    fn mov_mem_reg(&mut self, src: u32, dst_reg: Reg) {
        let val = self.get_mem(src);
        self.set_reg(dst_reg, val);
        self.ip += OpCode::MovMemReg.size();
        self.modified_regs.push(dst_reg);
        self.modified_regs.push(Reg::IP);
    }

    fn push_imm(&mut self, imm: u32) {
        self.sp -= 4;
        self.set_mem(self.sp, imm);
        self.ip += OpCode::PushImm.size();
        self.modified_regs.push(Reg::IP);
        self.modified_regs.push(Reg::SP);
    }

    fn push_reg(&mut self, reg: Reg) {
        let val = self.get_reg(reg);
        self.sp -= 4;
        self.set_mem(self.sp, val);
        self.ip += OpCode::PushReg.size();
        self.modified_regs.push(Reg::IP);
        self.modified_regs.push(Reg::SP);
    }

    fn push_mem(&mut self, addr: u32) {
        let val = self.get_mem(addr);
        self.sp -= 4;
        self.set_mem(self.sp, val);
        self.ip += OpCode::PushMem.size();
        self.modified_regs.push(Reg::IP);
        self.modified_regs.push(Reg::SP);
    }

    fn pop_reg(&mut self, reg: Reg) {
        let val = self.get_mem(self.sp);
        self.sp += 4;
        self.set_reg(reg, val);
        self.ip += OpCode::PopReg.size();
        self.modified_regs.push(reg);
        self.modified_regs.push(Reg::IP);
        self.modified_regs.push(Reg::SP);
    }

    fn pop_mem(&mut self, addr: u32) {
        let val = self.get_mem(self.sp);
        self.sp += 4;
        self.set_mem(addr, val);
        self.ip += OpCode::PopMem.size();
        self.modified_regs.push(Reg::IP);
        self.modified_regs.push(Reg::SP);
    }
}

impl std::fmt::Debug for CPU {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let header_line = "\x1B[34m┌──────┬──────────┐\x1B[0m";

        let sep = "\x1B[34m├──────┼──────────┤\x1B[0m";

        let footer_line = "\x1B[34m└──────┴──────────┘\x1B[0m";

        // Header
        writeln!(
            f,
            "{}\n{}\n{}",
            header_line, "\x1B[34m│\x1B[0m regs \x1B[34m│\x1B[0m content  \x1B[34m│\x1B[0m", sep
        )?;

        let special_regs = [(Reg::IP, self.ip), (Reg::SP, self.sp), (Reg::BP, self.bp)];

        for (reg, val) in special_regs.iter() {
            write!(f, "\x1B[34m│\x1B[0m {:?}   \x1B[34m│\x1B[0m", reg)?;
            // if register was modified during last instruction
            if self.modified_regs.contains(reg) {
                writeln!(f, " \x1B[33m{:08x}\x1B[0m \x1B[34m│\x1B[0m", val)?;
            } else if *val != 0 {
                writeln!(f, " {:08x} \x1B[34m│\x1B[0m", val)?;
            } else {
                writeln!(f, " \x1B[2m{:08x}\x1B[0m \x1B[34m│\x1B[0m", val)?;
            }
        }

        writeln!(f, "{}", sep)?;

        let general_regs = [
            (Reg::R1, self.r1),
            (Reg::R2, self.r2),
            (Reg::R3, self.r3),
            (Reg::R4, self.r4),
            (Reg::R5, self.r5),
            (Reg::R6, self.r6),
            (Reg::R7, self.r7),
            (Reg::R8, self.r8),
        ];

        for (reg, val) in general_regs.iter() {
            write!(f, "\x1B[34m│\x1B[0m {:?}   \x1B[34m│\x1B[0m", reg)?;
            // if register was modified during last instruction
            if self.modified_regs.contains(reg) {
                writeln!(f, " \x1B[33m{:08x}\x1B[0m \x1B[34m│\x1B[0m", val)?;
            } else if *val != 0 {
                writeln!(f, " {:08x} \x1B[34m│\x1B[0m", val)?;
            } else {
                writeln!(f, " \x1B[2m{:08x}\x1B[0m \x1B[34m│\x1B[0m", val)?;
            }
        }

        // Footer
        writeln!(f, "{}", footer_line)
    }
}
