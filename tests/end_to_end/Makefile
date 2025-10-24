CXX = riscv64-unknown-elf-g++
CC  = riscv64-unknown-elf-gcc
AS  = riscv64-unknown-elf-as

ASFLAGS   = -mabi=ilp32
CFLAGS    = -mabi=ilp32 -std=c11 -Wall -Wextra -I$(ASSETS_DIR)
CXXFLAGS  = -mabi=ilp32 -std=c++17 -Wall -Wextra -I$(ASSETS_DIR)
LDFLAGS   = -mabi=ilp32 -nostartfiles -nostdlib

SRC_DIR ?= ./

BUILD_DIR = $(SRC_DIR)/build

ASSETS_DIR = ../../assets
API_S = $(ASSETS_DIR)/api.s
API_O = $(BUILD_DIR)/api.o

.PHONY: all
all:

$(API_O): $(API_S)
	$(AS) $(ASFLAGS) -march=rv32i $< -o $@

$(BUILD_DIR)/_$(ARCH)%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) -march=$(ARCH) $< -o $@

$(BUILD_DIR)/%_$(ARCH).o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -march=$(ARCH) -c $< -o $@

$(BUILD_DIR)/%_$(ARCH).o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -march=$(ARCH) -c $< -o $@

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o $(API_O) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -march=$(ARCH) $^ -o $@

$(BUILD_DIR):
	mkdir -p ./$(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf ./$(BUILD_DIR)

.PHONY: test
test: $(BINARY)
	$(INTERPRETER) $(BINARY)

