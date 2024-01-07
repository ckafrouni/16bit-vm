#pragma once

#include <cstdint>

#include "instructions.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "addr.hpp"

namespace vm
{
    enum Mode
    {
        DEBUG,
        RELEASE,
    };

    class Interpreter
    {
    private:
        memory::Memory *mem;         // Pointer to memory
        registers::RegisterFile *rf; // Pointer to register file
        bool running;                // Flag indicating if the interpreter is running
        memory::Memory *program;     // Pointer to the program

    public:
        // Constructor
        Interpreter(memory::Memory *mem, registers::RegisterFile *rf)
            : mem(mem), rf(rf), running(false), program(nullptr) {}

        inline bool load(memory::Memory *program)
        {
            this->program = program;
            return true;
        }

        // Execution

        // Run the program with specified mode
        uint32_t run(Addr entry_point, Mode mode);

        // Fetchs the next opcode
        inline instructions::OpCode fetch()
        {
            auto op = (instructions::OpCode)this->program->read8(this->rf->get(registers::IP));
            this->rf->inc(registers::IP, 1);
            return op;
        }

        // Execute the specified opcode
        void execute(instructions::OpCode op);

        // Fetching and Reading

        // registers::RegisterEnum readRegister(memory::Memory *program, Addr address);
        registers::RegisterEnum readRegisterAtIP();
        // Addr readAddress(memory::Memory *program, Addr address);
        Addr readAddressAtIP();
        // uint32_t readLiteral(memory::Memory *program, Addr address);
        uint32_t readLiteralAtIP();

        // Writing

        // Write the value to the specified register
        void writeToReg(registers::RegisterEnum reg, uint32_t val);
        // Write the value to the specified memory address
        void writeToMem(Addr address, uint32_t val);
        // Push the value to the stack
        void push(uint32_t val);
        // Pop the value from the stack
        uint32_t pop();
    };

}
