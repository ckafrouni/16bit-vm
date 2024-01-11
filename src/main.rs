#![allow(dead_code)]

use crate::cpu::CPUMode;

mod byte_array;
mod cpu;

fn main() {
    let mut code = byte_array::ByteArray::new();

    // mov $0xdeadbeef, r8
    code.push8(cpu::OpCode::MovImmReg as u8);
    code.push32(0xdeadbeef);
    code.push8(cpu::Reg::R8 as u8);

    // mov r8, r5
    code.push8(cpu::OpCode::MovRegReg as u8);
    code.push8(cpu::Reg::R8 as u8);
    code.push8(cpu::Reg::R5 as u8);

    // TODO not implemented yet
    // // mov r5, (r8)
    // code.push8(cpu::OpCode::MovRegMem as u8);
    // code.push8(cpu::Reg::R5 as u8);
    // code.push8(cpu::Reg::R8 as u8);

    // TODO not implemented yet
    // // mov (r8), r6
    // code.push8(cpu::OpCode::MovMemReg as u8);
    // code.push8(cpu::Reg::R8 as u8);
    // code.push8(cpu::Reg::R6 as u8);

    // mov $0xcafebabe, r7
    code.push8(cpu::OpCode::MovImmReg as u8);
    code.push32(0xcafebabe);
    code.push8(cpu::Reg::R7 as u8);

    // halt
    code.push8(cpu::OpCode::Halt as u8);

    println!("\x1b[1;33m# Program:\x1b[0m");
    println!("{:?}", code.slice(0, code.size()));

    let mut cpu = cpu::CPU::default();
    // let res = cpu.run(CPUMode::DebugInteractive, &code);
    let res = cpu.run(CPUMode::DebugInteractive, &code);
    // let res = cpu.run(CPUMode::Release, &code);
    println!("\x1b[1;33m# Result:\x1b[0m {:?}", res);
    println!("{:?}", cpu)
}
