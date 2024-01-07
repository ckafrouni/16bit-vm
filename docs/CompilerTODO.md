# Compiler TODO generated from ChatGPT

## Message 1

To reorganize the `Compiler` class for extensibility and flexibility, consider the following strategies:

1. **Modularization of Encoding Functions**: You already have a good start by encapsulating encoding logic in methods like `encode_mov_lit_reg` and `encode_mov_reg_reg`. Continue this approach for all instruction types. This not only makes the code cleaner but also allows for easier modifications and extensions for new instructions.

2. **Abstract Syntax Tree (AST)**: Consider building an AST or an intermediate representation of the source code. This allows for more sophisticated manipulations, optimizations, and can make implementing multiple passes easier and more effective.

3. **Multiple Passes**: Implementing multiple passes can be very beneficial, especially for tasks like resolving labels and optimizing code. The first pass could parse the code and build the AST, resolving labels and syntax. A second pass could then generate the bytecode from the AST. Further passes could be used for optimization.

4. **Separation of Parsing and Encoding**: Decouple the parsing logic from the bytecode generation. The parsing phase should analyze the source code and produce a structured representation (like AST). The encoding phase should take this representation and generate the actual bytecode.

5. **Error Handling and Reporting**: Implement a robust error handling and reporting mechanism. This can include tracking line numbers and source positions for better error messages, which is especially useful during parsing and assembly.

6. **Utility Functions and Classes**: Encapsulate common tasks in utility functions or classes. For example, you could have a `Parser` class for parsing source code and a `BytecodeGenerator` class for encoding instructions.

7. **Extendibility for New Instructions**: Design your compiler in a way that adding new instructions requires minimal changes. This could be achieved through a plugin system or by following a design pattern like Command or Strategy, where each instruction type is handled by a separate class or function object.

8. **Testing and Validation**: Develop a comprehensive suite of unit tests to validate each part of your compiler. This is crucial for ensuring that extensions or modifications don’t introduce bugs.

9. **Documentation**: Maintain clear and thorough documentation, especially for the interfaces of your classes and methods. This is important for future extensions and maintenance.

10. **Use of Modern C++ Features**: Utilize modern C++ features like smart pointers (e.g., `std::unique_ptr`) instead of raw pointers for better memory management.

11. **Command-Line Interface (CLI) or GUI**: For user interaction, consider building a CLI or GUI. This can allow users to specify options for the compilation process, like optimization levels, output formats, etc.

Regarding whether you should implement multiple passes: Yes, multiple passes can significantly enhance the capabilities of your compiler. They allow for more complex and efficient processing of the source code, such as optimizing the output and better error handling. The first pass can focus on parsing and building an AST, while subsequent passes can perform optimization, code generation, and other tasks.

## Message 2

Certainly! Based on the reorganization strategy we discussed, I'll outline a header file for the compiler. This design will include classes for parsing, instruction encoding, and the main compiler class. It will make use of modern C++ practices and aim for extensibility and maintainability.

Here's an example header file:

```cpp
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <memory>

#include "memory.hpp"
#include "instruction.hpp" // You might need to create this for instruction representations

namespace compiler {

    // Forward declarations
    class Parser;
    class BytecodeGenerator;

    static const int initial_size = 1024; // 1KB

    class Compiler {
    private:
        std::unique_ptr<memory::Memory> program;
        std::map<std::string, Addr> labels;
        std::map<Addr, std::string> unresolved_labels;
        Addr current_addr = 0x00;

        std::unique_ptr<Parser> parser;
        std::unique_ptr<BytecodeGenerator> bytecodeGenerator;

    public:
        Compiler();
        ~Compiler();

        void assemble(const std::string& source);
        memory::Memory* get_program() const;
    };

    class Parser {
    public:
        std::vector<Instruction> parse(const std::string& source);
        // Add other parser-related methods here
    };

    class BytecodeGenerator {
    private:
        Compiler* compiler;

    public:
        explicit BytecodeGenerator(Compiler* comp);
        
        void generate(const std::vector<Instruction>& instructions);
        // Methods for encoding each instruction type
        Addr encode_mov_lit_reg(Addr addr, uint32_t lit, uint8_t reg);
        Addr encode_mov_reg_reg(Addr addr, uint8_t reg1, uint8_t reg2);
        // Add other encoding methods here
    };

    // Other utility functions and classes can be added here
}
```

This design assumes:

- A class `Instruction` (or a similar structure) represents individual instructions. This should be defined in `instruction.hpp`.
- `Parser` handles the parsing of source code and returns a structured representation, like an AST or a list of `Instruction` objects.
- `BytecodeGenerator` takes these instructions and generates the actual bytecode, writing to the `Compiler`'s memory.

You will need to implement the logic for these classes in the corresponding `.cpp` files. This design allows you to easily extend the compiler with new features, optimize the parsing and bytecode generation independently, and maintain a clear separation of concerns.
