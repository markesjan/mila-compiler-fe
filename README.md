# Mila Compiler Front-end

A modern C++23 front-end compiler for the Pascal-like Mila programming language, generating LLVM IR. Developed as a term project for BI-PJP at FIT CTU.

## Features

### Language Features
- **Data Types**: `integer` (32-bit signed), `real` (64-bit IEEE 754 float), `boolean` (`true`/`false`), and 1D arrays with arbitrary index ranges (`array [low..high] of <type>`).
- **Numeric Literals**: Decimal (`42`), octal (`&52`), hexadecimal (`$2A`), and floating-point (`3.14`, `1.5e-2`).
- **Declarations**: Compile-time constants (`const`), global and local variables (`var`).
- **Expressions & Operators**:
  - Arithmetic: `+`, `-`, `*`, `/` (real division), `div`, `mod`.
  - Relational: `=`, `<>`, `<`, `<=`, `>`, `>=`.
  - Logical: `and`, `or`, `not`, `xor`.
  - Implicit type promotion from integer to real in mixed-mode expressions.
- **Control Flow**:
  - Conditional branching: `if-then-else`.
  - Loops: `while-do`, `for-to-do`, and `for-downto-do`.
  - Loop controls: `break` and `continue`.
  - Early exit: `exit` statement.
- **Functions and Procedures**:
  - Value parameters and return values.
  - Local variable declarations and scoped symbols.
  - Recursion and indirect recursion with `forward;` declarations.
- **I/O Operations**: Built-in `readln`, `write`, and `writeln` for integers, real numbers, booleans, and string literals.

### Architecture & Design
- **Decoupled Architecture**: Recursive descent parser producing an AST, completely separated from LLVM IR generation.
- **RAII & Scope Safety**: Hierarchical `SymbolTable` with RAII `ScopeGuard` and `LoopGuard`.
- **Flyweight Type System**: Polymorphic `TypeImpl` wrapped by a value-type `Type` handle using C++20 concepts.
- **Modern Diagnostics**: C++23 `<print>` reporting with source line and column tracking.

## Requirements

- **C++ Compiler**: GCC 14+ or Clang 18+ with C++23 support
- **Build System**: CMake >= 3.28
- **LLVM**: LLVM 18 (`LLVMSupport`, `LLVMCore`, `LLVMIRReader`)
- **Backend Tools** (for full executable pipeline): `llc` and `clang`

## Build

Build the compiler using CMake:

```bash
cmake -B build
cmake --build build
```

The resulting compiler binary is created at `build/mila`.

## Usage and Testing

### Compiling to Native Executable
Use the root `mila` driver script to compile Mila source code into a native binary (runs front-end, `llc`, and `clang` linking with `src/fnc.c` runtime):

```bash
./mila samples/test1_custom.mila -o test1
./test1
```

### Direct LLVM IR Generation
Run the compiler binary directly to emit LLVM IR to standard output:

```bash
# Via file argument
./build/mila samples/factorial.mila

# Or via standard input
./build/mila < samples/factorial.mila
```

### Running Tests
Execute the automated test suite with CTest:

```bash
ctest --test-dir build --output-on-failure
```
