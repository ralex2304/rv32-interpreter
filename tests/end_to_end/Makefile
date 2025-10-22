CXX = riscv64-unknown-elf-g++
CC  = riscv64-unknown-elf-gcc
AS  = riscv64-unknown-elf-as

ASFLAGS   = -mabi=ilp32
CFLAGS    = -mabi=ilp32 -std=c11 -Wall -Wextra -I$(ASSETS_DIR)
CXXFLAGS  = -mabi=ilp32 -std=c++17 -Wall -Wextra -I$(ASSETS_DIR)
LDFLAGS   = -mabi=ilp32 -nostartfiles -nostdlib

ASSETS_DIR = ../../assets
API_S = $(ASSETS_DIR)/api.s
API_O = rv32i/api.o

.PHONY: all
all:

$(API_O): $(API_S)
	$(AS) $(ASFLAGS) -march=rv32i $< -o $@

rv32i/%.o: rv32i/%.c
	$(CC) $(CFLAGS) -march=rv32i -c $< -o $@

rv32i/%.o: rv32i/%.cpp
	$(CXX) $(CXXFLAGS) -march=rv32i -c $< -o $@

rv32i/%: rv32i/%.o $(API_O)
	$(CC) $(LDFLAGS) -march=rv32i $^ -o $@

.PHONY: test
test: $(BINARY)
	$(INTERPRETER) $(BINARY)

