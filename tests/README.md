# RV32 interpreter tests

Currently there are 2 sets of tests. All tests are listed in `CMakeLists.txt` in this directory

## Build and run

You can build and run tests from top `CMakeLists` or from this directory. In later case:

Build only:

```bash
cd tests
cmake -S . -B build
cmake --build build
```

To run the tests you must specify interpreter path in `INTERPRETER` variable

Build and run:

```bash
cd tests
cmake -S. -B build -DINTERPRETER=../build/rv32-interpreter
cmake --build build

ctest --build-dir build
```

## Task tests

Tests supplied with the task are in [riscv-interpreter-task](https://gitlab.com/iDang3r/riscv-interpreter-task) submodule

Submodule has its own Makefile to build and run tests

## End to end tests

End to end tests are built by `test_build.py` and run by `test_runner.py`. Each test can have several configurations, listed in its directory

### Test sources

Each test can contain 1 `.c`, `.cpp` or `.s` source file. The source file is built and linked with `assets/api.s`. You can use headers from `assets/` in source files (`assets/api.h`, `assets/lib.h`)

### Test config

`-march` options list for this test must be in `build_config.txt`. E.g:

```
build_config.txt
===
rv32i rv32imf_zbb
```

Next, there can be several test run configs. Config files for each must start with `{n}_` prefix, where `{n}` is number of test config starting from 1

Each consists of 1 to 4 files:

- `{n}_config.txt` - run configuration: `march`, `argv`, return code
- `{n}_stdin.{txt|bin}` [optional] - contents of this file will be sent to process
- `{n}_stdout.{txt|bin}` [optional] - process's `stdout` will be compared with contents of this file
- `{n}_stderr.{txt|bin}` [optional] - same with `stderr`

Not specifying `{n}_stdin.{txt|bin}`, `{n}_stdout.{txt|bin}` or `{n}_stderr.{txt|bin}` is equivalent to them being empty

`.txt` and `.bin` are equivalent and used only to hint people how to treat contents of files.

`{n}_config.txt` must contain this 3 parameters each on new line:
- List of `-march` options for this test to be run with. Only options that were listed in `build_config.txt` are possible. You may append `+` to last option. It will automatically expand to following after given option arches from the build config. E.g `rv32im+` can be expanded to `rv32im rv32imf rv32imf_zbb`
- Command line arguments that will be passed to process
- Return code of process (0-255). It will be compared with returned code of process

E.g:

```
1_config.txt
===
rv32i rv32imf+
123 -i abc
0
```

