use crate::byte_array;
use crate::cpu::op_code::{OpCode, Operand};
use crate::cpu::reg::Reg;

#[derive(Clone, Copy, PartialEq, Debug)]
pub enum CPUState {
    Running,
    Halted,
}

impl Default for CPUState {
    fn default() -> Self {
        CPUState::Halted
    }
}

pub enum CPUMode {
    Debug,            // Debug mode : will print debug info
    DebugInteractive, // Debug mode : will print debug info and wait for user input
    Release,
}

#[derive(Debug)]
pub enum CPUResult {
    Halt(u32),
    Error(CPUError),
}

#[derive(Debug)]
pub enum CPUError {
    IPOutOfBounds,
    CPUNotRunning,
}

#[derive(Default)]
pub struct CPU {
    state: CPUState,

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
}

// Contains methods for reading and writing registers
// and memory, along with other attributes
impl CPU {
    pub fn set_state(&mut self, state: CPUState) {
        self.state = state;
    }

    pub fn get_state(&self) -> CPUState {
        self.state
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

    #[allow(unused)]
    pub fn set_mem(&mut self, addr: u32, val: u32) {
        todo!("set_mem");
    }

    #[allow(unused)]
    pub fn get_mem(&self, addr: u32) -> u32 {
        todo!("get_mem");
    }
}

// Contains methods for stepping through the code
impl CPU {
    pub fn run(&mut self, mode: CPUMode, code: &byte_array::ByteArray) -> CPUResult {
        match mode {
            CPUMode::Debug => self.run_debug(code),
            CPUMode::DebugInteractive => self.run_debug_interactive(code),
            CPUMode::Release => self.run_release(code),
        }
    }

    pub fn run_debug(&mut self, code: &byte_array::ByteArray) -> CPUResult {
        let mut _n_ops = 0;
        self.state = CPUState::Running;

        println!("Starting CPU state:");
        println!("{:?}", self);

        while self.state == CPUState::Running {
            _n_ops += 1;
            match self.fetch(code) {
                Err(err) => return CPUResult::Error(err),
                Ok((op, operands)) => {
                    println!(
                        "Step {:02}\nIP: {:08x} - {:?} {:?}",
                        _n_ops, self.ip, op, operands
                    );
                    self.execute(op, operands);
                    println!("{:?}", self);
                }
            }
        }
        return CPUResult::Halt(self.r1);
    }

    pub fn run_debug_interactive(&mut self, code: &byte_array::ByteArray) -> CPUResult {
        let mut _n_ops = 0;
        self.state = CPUState::Running;

        println!("Starting CPU state:");
        println!("{:?}", self);

        while self.state == CPUState::Running {
            _n_ops += 1;

            match self.fetch(code) {
                Err(err) => return CPUResult::Error(err),
                Ok((op, operands)) => {
                    println!(
                        "Step {:02}\nIP: {:08x} - {:?} {:?}",
                        _n_ops, self.ip, op, operands
                    );

                    println!("\x1b[1;33m# Press enter to continue...\x1b[0m");
                    let mut _input = String::new();
                    std::io::stdin()
                        .read_line(&mut _input)
                        .expect("Failed to read line");

                    self.execute(op, operands);

                    println!("{:?}", self);
                }
            }
        }
        return CPUResult::Halt(self.r1);
    }

    pub fn run_release(&mut self, code: &byte_array::ByteArray) -> CPUResult {
        let mut _n_ops = 0;
        self.state = CPUState::Running;
        while self.state == CPUState::Running {
            _n_ops += 1;

            match self.fetch(code) {
                Err(err) => return CPUResult::Error(err),
                Ok((op, operands)) => {
                    println!(
                        "Step {:02}\nIP: {:08x} - {:?} {:?}",
                        _n_ops, self.ip, op, operands
                    );
                    self.execute(op, operands)
                }
            }
        }
        return CPUResult::Halt(self.r1);
    }
}

impl CPU {
    pub fn fetch(
        &mut self,
        code: &byte_array::ByteArray,
    ) -> Result<(OpCode, Vec<Operand>), CPUError> {
        if self.state != CPUState::Running {
            return Err(CPUError::CPUNotRunning);
        }

        if self.ip >= code.size() {
            self.state = CPUState::Halted;
            return Err(CPUError::IPOutOfBounds);
        }

        let mut ip = self.ip;

        let op = OpCode::from(code.get8(ip as u32));
        ip += 1;

        match op {
            OpCode::Halt => Ok((OpCode::Halt, [Operand::None].to_vec())),

            OpCode::MovImmReg => {
                let imm = code.get32(ip);
                ip += 4;

                let dst_reg: Reg = Reg::from(code.get8(ip));

                Ok((
                    OpCode::MovImmReg,
                    [Operand::Imm(imm), Operand::Reg(dst_reg)].to_vec(),
                ))
            }

            OpCode::MovRegReg => {
                let src_reg = Reg::from(code.get8(ip));
                ip += 1;

                let dst_reg = Reg::from(code.get8(ip));

                Ok((
                    OpCode::MovRegReg,
                    [Operand::Reg(src_reg), Operand::Reg(dst_reg)].to_vec(),
                ))
            }

            OpCode::MovRegMem => {
                let src_reg = Reg::from(code.get8(ip));
                ip += 1;

                let dst_addr: u32 = code.get32(ip);

                Ok((
                    OpCode::MovRegMem,
                    [Operand::Reg(src_reg), Operand::Mem(dst_addr)].to_vec(),
                ))
            }

            OpCode::MovMemReg => {
                let src_addr = code.get32(ip);
                ip += 4;

                let dst_reg = Reg::from(code.get8(ip));

                Ok((
                    OpCode::MovMemReg,
                    [Operand::Mem(src_addr), Operand::Reg(dst_reg)].to_vec(),
                ))
            }

            OpCode::PushImm => {
                let imm = code.get32(ip);

                Ok((OpCode::PushImm, [Operand::Imm(imm)].to_vec()))
            }

            OpCode::PushReg => {
                let reg = Reg::from(code.get8(ip));

                Ok((OpCode::PushReg, [Operand::Reg(reg)].to_vec()))
            }

            OpCode::PushMem => {
                let addr = code.get32(ip);

                Ok((OpCode::PushMem, [Operand::Mem(addr)].to_vec()))
            }

            OpCode::PopReg => {
                let reg = Reg::from(code.get8(ip));

                Ok((OpCode::PopReg, [Operand::Reg(reg)].to_vec()))
            }

            OpCode::PopMem => {
                let addr = code.get32(ip);

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
    }

    fn mov_imm_reg(&mut self, imm: u32, dst_reg: Reg) {
        self.set_reg(dst_reg, imm);
        self.ip += OpCode::MovImmReg.size();
    }

    fn mov_reg_reg(&mut self, src_reg: Reg, dst_reg: Reg) {
        let val = self.get_reg(src_reg);
        self.set_reg(dst_reg, val);
        self.ip += OpCode::MovRegReg.size();
    }

    fn mov_reg_mem(&mut self, src_reg: Reg, dst: u32) {
        let val = self.get_reg(src_reg);
        self.set_mem(dst, val);
        self.ip += OpCode::MovRegMem.size();
    }

    fn mov_mem_reg(&mut self, src: u32, dst_reg: Reg) {
        let val = self.get_mem(src);
        self.set_reg(dst_reg, val);
        self.ip += OpCode::MovMemReg.size();
    }

    fn push_imm(&mut self, imm: u32) {
        self.sp -= 4;
        self.set_mem(self.sp, imm);
        self.ip += OpCode::PushImm.size();
    }

    fn push_reg(&mut self, reg: Reg) {
        let val = self.get_reg(reg);
        self.sp -= 4;
        self.set_mem(self.sp, val);
        self.ip += OpCode::PushReg.size();
    }

    fn push_mem(&mut self, addr: u32) {
        let val = self.get_mem(addr);
        self.sp -= 4;
        self.set_mem(self.sp, val);
        self.ip += OpCode::PushMem.size();
    }

    fn pop_reg(&mut self, reg: Reg) {
        let val = self.get_mem(self.sp);
        self.sp += 4;
        self.set_reg(reg, val);
        self.ip += OpCode::PopReg.size();
    }

    fn pop_mem(&mut self, addr: u32) {
        let val = self.get_mem(self.sp);
        self.sp += 4;
        self.set_mem(addr, val);
        self.ip += OpCode::PopMem.size();
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
            if *val != 0 {
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
            if *val != 0 {
                writeln!(f, " {:08x} \x1B[34m│\x1B[0m", val)?;
            } else {
                writeln!(f, " \x1B[2m{:08x}\x1B[0m \x1B[34m│\x1B[0m", val)?;
            }
        }

        // Footer
        writeln!(f, "{}", footer_line)
    }
}
