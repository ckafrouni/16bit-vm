use std::env;

mod utils;
mod vm;

fn make_program() -> vm::device::ByteArray {
    let mut prog = vm::device::ByteArray::new();

    // mov $0xdeadbeef, r8
    prog.push8(vm::OpCode::MovImmReg as u8);
    prog.push32(0xdeadbeef);
    prog.push8(vm::Reg::R8 as u8);

    // mov r8, r5
    prog.push8(vm::OpCode::MovRegReg as u8);
    prog.push8(vm::Reg::R8 as u8);
    prog.push8(vm::Reg::R5 as u8);

    // push r5
    prog.push8(vm::OpCode::PushReg as u8);
    prog.push8(vm::Reg::R5 as u8);

    // TODO not implemented yet
    // // mov r5, (r8)
    // prog.push8(vm::OpCode::MovRegMem as u8);
    // prog.push8(vm::Reg::R5 as u8);
    // prog.push8(vm::Reg::R8 as u8);

    // TODO not implemented yet
    // // mov (r8), r6
    // prog.push8(vm::OpCode::MovMemReg as u8);
    // prog.push8(vm::Reg::R8 as u8);
    // prog.push8(vm::Reg::R6 as u8);

    // mov $0xcafebabe, r7
    prog.push8(vm::OpCode::MovImmReg as u8);
    prog.push32(0xcafebabe);
    prog.push8(vm::Reg::R7 as u8);

    // mov $0x12345678, r1
    prog.push8(vm::OpCode::MovImmReg as u8);
    prog.push32(0x12345678);
    prog.push8(vm::Reg::R1 as u8);

    // halt
    prog.push8(vm::OpCode::Halt as u8);

    prog
}

fn main() {
    let mem_size = 0x0000_0400; // 1 KiB
    let prog_start = 0x0000_0000;
    let stack_start = mem_size - 1;
    let stack_size = 0x0000_0100; // 256 B

    let mut mmu = vm::MMU::new();

    let prog = make_program();
    println!("\x1b[1;33m# Program:\x1b[0m");
    println!("{:?}", prog.slice(0, prog.size()));
    let _ = mmu.map(
        Box::new(prog.clone()),
        prog_start,
        prog.size(),
        vm::AccessFlags::ReadExecute,
    );

    let stack = vm::device::ByteArray::new_with_size(stack_size);
    println!("# Stack:");
    println!("{:?}", stack.slice(0, stack.size()));
    let _ = mmu.map(
        Box::new(stack),
        stack_start - stack_size,
        stack_size,
        vm::AccessFlags::ReadWrite,
    );

    println!("{:?}", mmu);

    let mut cpu = vm::CPU::new(mmu);
    cpu.set_reg(vm::Reg::IP, prog_start);
    cpu.set_reg(vm::Reg::SP, stack_start);

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

    println!("\x1b[1;33m# CPU:\x1b[0m");
    println!("{:?}", cpu);
    println!("\x1b[1;33m# Stack:\x1b[0m");
    println!(
        "{:?}",
        vm::device::ByteArray::from(
            cpu.slice(stack_start - stack_size, stack_start).unwrap(),
            stack_start - stack_size
        )
    );

    println!(
        "{:?}",
        vm::device::ByteArray::from(cpu.slice(0, mem_size).unwrap(), 0)
    );
    println!("\x1b[1;33m# Result:\x1b[0m {:?}", res);
}
