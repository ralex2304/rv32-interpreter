#include "rvi_instruction_registry.h"
#include "rvi_instruction_sets/rv32i/rv32i.h"

int main() {

    rvi::InstructionRegistry registry;
    rvi::rv32i::add_instructions(registry);

}

