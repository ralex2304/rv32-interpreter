#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"

#include <cassert>
#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace rvi {

class InstructionRegistry {
    public:
        void add_instruction(std::unique_ptr<Instruction> instruction) {
            using namespace std::literals;

            auto [ext_opcode, ext_opcode_type] = instruction->get_extended_opcode();
            auto instr_name = instruction->get_name();

            size_t type = static_cast<size_t>(ext_opcode_type);

            auto [it, inserted] = registries_.at(type).emplace(ext_opcode, std::move(instruction));
            if (!inserted)
                throw std::runtime_error("Error adding instruction \""s + std::string(instr_name) +
                                                                        "\": already exist"s);
        }

        const Instruction* get_instruction(RawInstruction raw_instruction) const {
            ExtendedOpcodesFactory ext_opcodes_factory(raw_instruction);

            auto ext_opcode = ext_opcodes_factory.begin();
            auto registry = registries_.begin();

            while (ext_opcode != ext_opcodes_factory.end() && registry != registries_.end()) {
                auto it = registry->find(*ext_opcode);

                if (it != registry->end()) {
                    return it->second.get();
                }

                ext_opcode++;
                registry++;
            }
            assert(ext_opcode == ext_opcodes_factory.end());
            assert(registry == registries_.end());

            return nullptr;
        }

    private:
        std::array<std::unordered_map<RawInstruction, std::unique_ptr<Instruction>>,
                   EXTENDED_OPCODES_TYPES> registries_;
};

} // namespace rvi

