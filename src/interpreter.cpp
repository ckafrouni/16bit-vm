#include <iostream>
#include <iomanip>
#include <string>

#include "interpreter.hpp"

#include "utils/fmt.hpp"
#include "utils/hexutils.hpp"

#include "instructions.hpp"
#include "memory.hpp"
#include "registers.hpp"

using namespace instructions;
using namespace vm;

void Interpreter::push(uint32_t val)
{
    this->rf->dec(registers::SP, 4);
    this->mem->write32(this->rf->get(registers::SP), val);
}

uint32_t Interpreter::pop()
{
    auto val = this->mem->read32(this->rf->get(registers::SP));
    this->rf->inc(registers::SP, 4);
    return val;
}

Addr Interpreter::readAddressAtIP()
{
    auto addr = this->program->read32(this->rf->get(registers::IP));
    this->rf->inc(registers::IP, 4);
    return addr;
}

registers::RegisterEnum Interpreter::readRegisterAtIP()
{
    auto reg = (registers::RegisterEnum)this->program->read8(this->rf->get(registers::IP));
    this->rf->inc(registers::IP, 1);
    return reg;
}

uint32_t Interpreter::readLiteralAtIP()
{
    auto lit = this->program->read32(this->rf->get(registers::IP));
    this->rf->inc(registers::IP, 4);
    return lit;
}

void Interpreter::writeToReg(registers::RegisterEnum reg, uint32_t val)
{
    this->rf->set(reg, val);
}

void Interpreter::writeToMem(Addr address, uint32_t val)
{
    this->mem->write32(address, val);
}

void Interpreter::execute(OpCode op)
{
    switch (op)
    {
    case HALT:
    {
        running = false;
        return;
    }

    /** Stack **/
    case PUSH_LIT:
    {
        auto lit = readLiteralAtIP();
        push(lit);
        return;
    }
    case PUSH_REG:
    {
        auto reg = readRegisterAtIP();
        push(this->rf->get(reg));
        return;
    }
        // TODO PUSH_MEM

    case POP_REG:
    {
        auto reg = readRegisterAtIP();
        writeToReg(reg, pop());
        return;
    }
    // TODO POP_MEM

    /** registers::RegisterEnum **/
    case MOV_LIT_REG:
    {
        auto lit = readLiteralAtIP();
        auto reg = readRegisterAtIP();
        writeToReg(reg, lit);
        return;
    }
    case MOV_REG_REG:
    {
        auto src = readRegisterAtIP();
        auto dst = readRegisterAtIP();
        writeToReg(dst, this->rf->get(src));
        return;
    }
    case MOV_MEM_REG:
    {
        Addr addr = readAddressAtIP();
        auto reg = readRegisterAtIP();
        writeToReg(reg, this->mem->read32(addr));
        return;
    }

    /** memory::Memory **/
    case MOV_LIT_MEM:
    {
        auto lit = readLiteralAtIP();
        Addr addr = readAddressAtIP();
        writeToMem(addr, lit);
        return;
    }
    case MOV_REG_MEM:
    {
        auto reg = readRegisterAtIP();
        Addr addr = readAddressAtIP();
        writeToMem(addr, this->rf->get(reg));
        return;
    }
    case MOV_MEM_MEM:
    {
        auto src = readAddressAtIP();
        auto dst = readAddressAtIP();
        writeToMem(dst, this->mem->read32(src));
        return;
    }

    /** Arithmetic **/
    case ADD_LIT_REG:
    {
        auto lit = readLiteralAtIP();
        auto reg = readRegisterAtIP();
        writeToReg(reg, this->rf->get(reg) + lit);
        return;
    }
    case ADD_REG_REG_REG:
    {
        auto dst = readRegisterAtIP();

        auto srca = readRegisterAtIP();
        auto srcb = readRegisterAtIP();

        writeToReg(dst, this->rf->get(srca) + this->rf->get(srcb));
        return;
    }
    case SUB_LIT_REG:
    {
        auto lit = readLiteralAtIP();
        auto reg = readRegisterAtIP();
        writeToReg(reg, this->rf->get(reg) - lit);
        return;
    }
    case SUB_REG_REG:
    {
        auto src = readRegisterAtIP();
        auto dst = readRegisterAtIP();
        writeToReg(dst, this->rf->get(dst) - this->rf->get(src));
        return;
    }
    case INC_REG:
    {
        auto reg = readRegisterAtIP();
        writeToReg(reg, this->rf->get(reg) + 1);
        return;
    }
    case INC_MEM:
    {
        Addr addr = readAddressAtIP();
        writeToMem(addr, this->mem->read32(addr) + 1);
        return;
    }
    case DEC_REG:
    {
        auto reg = readRegisterAtIP();
        writeToReg(reg, this->rf->get(reg) - 1);
        return;
    }
    case DEC_MEM:
    {
        Addr addr = readAddressAtIP();
        writeToMem(addr, this->mem->read32(addr) - 1);
        return;
    }

    /** Jumps **/
    case JMP:
    {
        Addr addr = readAddressAtIP();
        writeToReg(registers::IP, addr);
        return;
    }
    case JMP_NE: // JMP_NE %<reg> %<reg> <addr>
    {
        auto regx = readRegisterAtIP();
        auto regy = readRegisterAtIP();
        Addr addr = readAddressAtIP();
        if (this->rf->get(regx) != this->rf->get(regy))
            writeToReg(registers::IP, addr);
        // if (this->rf->get(registers::ACC) != this->rf->get(reg))
        //     writeToReg(registers::IP, addr);
        return;
    }
    case JMP_EQ: // JMP_EQ Rx <addr>
    {
        auto reg = readRegisterAtIP();
        Addr addr = readAddressAtIP();
        if (this->rf->get(registers::ACC) == this->rf->get(reg))
            this->rf->set(registers::IP, addr);
        return;
    }
    case JMP_GT: // JMP_GT Rx <addr>
    {
        auto reg = readRegisterAtIP();
        Addr addr = readAddressAtIP();
        if (this->rf->get(registers::ACC) > this->rf->get(reg))
            this->rf->set(registers::IP, addr);
        return;
    }
    case JMP_GE: // JMP_GTE Rx <addr>
    {
        auto reg = readRegisterAtIP();
        Addr addr = readAddressAtIP();
        if (this->rf->get(registers::ACC) >= this->rf->get(reg))
            this->rf->set(registers::IP, addr);
        return;
    }
    case JMP_LT: // JMP_LT Rx <addr>
    {
        auto reg = readRegisterAtIP();
        Addr addr = readAddressAtIP();
        if (this->rf->get(registers::ACC) < this->rf->get(reg))
            this->rf->set(registers::IP, addr);
        return;
    }
    case JMP_LE: // JMP_LTE Rx <addr>
    {
        auto reg = readRegisterAtIP();
        Addr addr = readAddressAtIP();
        if (this->rf->get(registers::ACC) <= this->rf->get(reg))
            this->rf->set(registers::IP, addr);
        return;
    }

    /** Subroutines **/
    case CALL:
    {
        // TODO after implementing the stack
        return;
    }
    case RETURN:
    {
        // TODO after implementing the stack and CALL
        return;
    }
    }

    throw std::runtime_error(utils::colorize("Unknown opcode: <" + std::to_string(op) + ">", utils::FG_RED, utils::BOLD));
}

uint32_t Interpreter::run(Addr entry_point, Mode mode)
{
    std::cout << colorize("## START OF PROGRAM", utils::Colors::FG_RED, utils::Styles::BOLD) << std::endl;
    std::cout << colorize("## Entry point: " + utils::hexstr32(entry_point), utils::Colors::FG_WHITE, utils::Styles::BOLD) << std::endl;
    std::cout << colorize("## Mode: " + std::string(mode == Mode::DEBUG ? "DEBUG" : "RELEASE"), utils::Colors::FG_WHITE, utils::Styles::BOLD) << std::endl;

    this->rf->set(registers::IP, entry_point);
    this->running = true;

    if (mode != Mode::DEBUG)
    {
        while (running)
        {
            auto op = fetch();
            execute(op);
        }
    }
    else
    {
        registers::inspect(this->rf);
        std::cout << utils::colorize("## BREAK (Press a key to start)", utils::Colors::FG_YELLOW, utils::Styles::BOLD);
        std::cin.get();

        while (running)
        {
            this->rf->reset_modified();

            auto op = fetch();

            std::cout << std::endl;
            std::cout << utils::colorize("## Next instruction: " + to_string(op), utils::Colors::FG_WHITE, utils::Styles::BOLD) << std::endl;
            std::cout << utils::colorize("## BREAK (Press a key to continue)", utils::Colors::FG_YELLOW, utils::Styles::BOLD);
            std::cin.get();

            execute(op);
            registers::inspect(this->rf);
        }
    }

    auto ret = this->rf->get(registers::ACC);

    std::cout << colorize("## END OF PROGRAM", utils::Colors::FG_RED, utils::Styles::BOLD) + " "
              << colorize("registers::ACC = " + utils::hexstr32(ret), utils::Colors::FG_WHITE, utils::Styles::BOLD) << std::endl;

    return ret;
}
