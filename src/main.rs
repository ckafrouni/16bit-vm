use std::env;
use vm::{
    device::{self, ByteArray},
    OpCode::*,
    Reg::*,
};

use crate::vm::device::Device;

mod utils;
mod vm;

fn make_program() -> ByteArray {
    let mut prog = ByteArray::new();

    // mov $0xdeadbeef, r8
    prog.push8(MovImmReg as u8);
    prog.push32(0xdeadbeef);
    prog.push8(R8 as u8);

    // mov r8, r5
    prog.push8(MovRegReg as u8);
    prog.push8(R8 as u8);
    prog.push8(R5 as u8);

    // push r5
    prog.push8(PushReg as u8);
    prog.push8(R5 as u8);

    // pop r6
    prog.push8(PopReg as u8);
    prog.push8(R6 as u8);

    // push r5
    prog.push8(PushReg as u8);
    prog.push8(R5 as u8);

    // mov $0x00000200, r4
    prog.push8(MovImmReg as u8);
    prog.push32(0x00000200);
    prog.push8(R4 as u8);

    // // TODO not implemented yet
    // // mov (r4), r5
    // prog.push8(MovMemReg as u8);
    // prog.push8(R4 as u8);
    // prog.push8(R5 as u8);

    // mov $0xcafebabe, r7
    prog.push8(MovImmReg as u8);
    prog.push32(0xcafebabe);
    prog.push8(R7 as u8);

    // mov $0x12345678, r1
    prog.push8(MovImmReg as u8);
    prog.push32(0x12345678);
    prog.push8(R1 as u8);

    // halt
    prog.push8(Halt as u8);

    prog
}

fn main() {
    // Memory layout:
    // 0x0000_0000 - 0x0000_00ff: program (256 B)
    // 0x0000_0100 - 0x0000_01ff: data (256 B)
    // 0x0000_0200 - 0x0000_02ff: heap (256 B)
    // 0x0000_0300 - 0x0000_03ff: UNUSED (256 B)
    // 0x0000_0400 - 0x0000_04ff: stack (256 B)
    // Total: 1024 B mapped

    let mut mmu = vm::MMU::new();

    // Program
    let _ = mmu.map(
        Box::new(make_program()),
        0x0000_0000,
        0x0000_0100,
        vm::AccessFlags::ReadExecute,
    );

    // Data
    let mut data = ByteArray::new_with_size(0x0000_0100); // 256 B
    data.push32(0xc0de_cafe);
    let _ = mmu.map(
        Box::new(data),
        0x0000_0100,
        0x0000_0100,
        vm::AccessFlags::ReadWrite,
    );

    // Heap
    let mut heap = ByteArray::new_with_size(0x0000_0100); // 256 B
    heap.push32(0xdeaf_beef);
    let _ = mmu.map(
        Box::new(heap),
        0x0000_0200,
        0x0000_0100,
        vm::AccessFlags::ReadWrite,
    );

    // Stack
    let _ = mmu.map(
        Box::new(ByteArray::new_with_size(0x0000_0100)), // 256 B
        0x0000_0400,
        0x0000_0100,
        vm::AccessFlags::ReadWrite,
    );

    println!("[{}:{}] # MMU: {:?}", file!(), line!(), mmu);

    let mut stdout = device::Stdout::new();
    let s = "Hello, world!\n";
    stdout.write(0x0000_0000, s.as_bytes()).unwrap();

    let mut cpu = vm::CPU::new(mmu);
    cpu.set_reg(IP, 0x0000_0000);
    cpu.set_reg(SP, 0x0000_0500);

    let mode = match env::var("MODE") {
        Ok(val) => {
            if val == "int" {
                vm::CPUMode::DebugInteractive
            } else if val == "dbg" {
                vm::CPUMode::Debug
            } else {
                vm::CPUMode::Release
            }
        }
        Err(_) => vm::CPUMode::Release,
    };

    let res = cpu.run(mode).unwrap();

    // println!("[{}:{}] # CPU:", file!(), line!());
    // println!("{:?}", cpu);
    println!("\x1b[36m[{}:{}] # Stack:", file!(), line!());
    println!("{:?}", cpu.view_window(0x0000_04f0, 0x0000_0500));
    // println!(
    //     "{:?}",
    //     ByteArray::from(cpu.slice(stack_start, stack_end - 1).unwrap(), stack_start)
    // );
    // println!("[{}:{}] # Heap:", file!(), line!());
    // println!(
    //     "{:?}",
    //     ByteArray::from(
    //         cpu.slice(0x0000_0100, 0x0000_0100 + 0x0000_0100).unwrap(),
    //         0x0000_0100
    //     )
    // );

    println!("\x1b[36m[{}:{}] # Result: {}\x1b[0m", file!(), line!(), res);

    println!("{}", cpu.view_regs());
}
