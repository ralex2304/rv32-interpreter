#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"

#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace rvi {

class InstructionRegistry {
    public:
        void add_instruction(std::unique_ptr<Instruction> instruction) {
            auto ext_opcode = instruction->get_extended_opcode();

            auto [it, inserted] = registry_.emplace(ext_opcode, std::move(instruction));
            if (!inserted)
                throw std::runtime_error("Error adding opcode: already exist");
        }

        const Instruction* get_instruction(RawInstruction raw_instruction) const {
            ExtendedOpcodesFactory ext_opcodes_factory(raw_instruction);

            for (auto ext_opcode: ext_opcodes_factory) {
                auto it = registry_.find(ext_opcode);

                if (it != registry_.end()) {
                    return it->second.get();
                }
            }

            return nullptr;
        }

    private:
        std::unordered_map<RawInstruction, std::unique_ptr<Instruction>> registry_;
};

} // namespace rvi

