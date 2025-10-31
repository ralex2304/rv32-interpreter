# RV32 interpreter tests

Currently there are 2 sets of tests. All tests are listed in `CMakeLists.txt` in this directory

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
- `{n}_stdin.txt` [optional] - contents of this file will be sent to process
- `{n}_stdout.txt` [optional] - process's `stdout` will be compared with contents of this file
- `{n}_stderr.txt` [optional] - same with `stderr`

Not specifying `{n}_stdin.txt`, `{n}_stdout.txt` or `{n}_stderr.txt` is equivalent to them being empty

`{n}_config.txt` must contain this 3 parameters each on new line:
- List of `-march` options for this test to be run with. Only options that were listed in `build_config.txt` are possible
- Command line arguments that will be passed to process
- Return code of process (0-255). It will be compared with returned code of process

E.g:

```
1_config.txt
===
rv32i rv32imf_zbb
123 -i abc
0
```

