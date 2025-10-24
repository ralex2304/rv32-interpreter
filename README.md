# RV32 interpreter

32-bit RISC-V interpreter

MIPT Baikal Electronics chair Compiler Technologies course task

## Supported instruction sets and extensions

- `RV32I` - base integer

TODO:

- `RV32M` - integer multiplication and division
- `RV32F` - single-precision floating-point
- `RV32Zbb` - basic bit manipulation

### Syscalls

`ecall` instruction supports these Linux system calls:
- `read` - `63`
- `write` - `64`
- `exit` - `93`

## Build dependencies

- `C++ 23` compiler
- `CMake`

## Test dependencies

- `riscv64-unknown-elf` toolchain
- `Python`

## Build

```bash
cmake -S . -B <build_dir> -DCMAKE_BUILD_TYPE={Debug, Release} [-DUSE_SANITIZER='Address;Undefined']
cmake --build build
```

E.g:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZER='Address;Undefined'
cmake --build build
```

Tests build

```bash
cd tests/end_to_end
make <path_to_target_executable (source name without extension)>
```

E.g:

```bash
make rv32i/fibonacci/test
```

## Usage

Simple mode (without interpreter arguments):

```bash
./build/rv32-interpreter <path-to-rv32-elf> <target-program-arguments...>
===
./build/rv32-interpreter tests/end_to_end/rv32i/fibonacci/test 5
```

Complex mode (with interpreter arguments):

```bash
./build/rv32-interpreter <flags> -- <path-to-rv32-elf> <target-program-arguments...>
===
./build/rv32-interpreter -v INFO --log=log.dump -- factorial 5
```

### Arguments

```
-v, --verbosity arg  stderr messages verbosity: FATAL, ERROR, WARNING,
                     INFO, DUMP (default: ERROR)
-l, --log arg        Create log file. You can specify verbosity as
                     filename extension (default: DUMP). E.g log.error
-h, --help           Print help

```
## Tests

Use CMake CTest to run tests

```bash
ctest --build-dir build
```

[tests/README.md](tests) contains some details about tests

