#include "rvi_instruction_registry.h"

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"

#include <cassert>
#include <utility>

using namespace rvi;

template<typename Variant, size_t... Indexes>
bool InstructionRegistry::try_emplace_instruction_impl_(const Variant& ext_opcode,
                                                        std::unique_ptr<Instruction> instruction,
                                                        std::index_sequence<Indexes...>) {

    bool is_inserted = ([&registries = registries_, ext_opcode, &instruction]() {
            const auto& ext_opcode_value = std::get_if<Indexes>(&ext_opcode);

            if (ext_opcode_value == nullptr)
                return true;

            auto& registry = std::get<Indexes>(registries);

            auto [it, inserted] = registry.try_emplace(*ext_opcode_value, std::move(instruction));

            return inserted;
        }() && ...);

    return is_inserted;
}

template<typename... Types>
bool InstructionRegistry::try_emplace_instruction_(const std::variant<Types...>& ext_opcode,
                                                   std::unique_ptr<Instruction> instruction) {
    return try_emplace_instruction_impl_(ext_opcode, std::move(instruction),
                                         std::make_index_sequence<sizeof...(Types)>{});
}

void InstructionRegistry::add_instruction(std::unique_ptr<Instruction> instruction) {
    using namespace std::literals;

    ExtendedOpcode ext_opcode = instruction->get_extended_opcode();
    auto instr_name = instruction->get_name();

    if (!try_emplace_instruction_(ext_opcode, std::move(instruction)))
        throw exception("Error adding instruction \""s + std::string(instr_name) +
                        "\": already exist"s);
}

template<typename Tuple, size_t Index>
const Instruction* InstructionRegistry::search_in_registry_(const Tuple& ext_opcodes) const {
    const auto& registry = std::get<Index>(registries_);

    auto it = registry.find(std::get<Index>(ext_opcodes));

    if (it != registry.end()) {
        return it->second.get();
    }

    return nullptr;
}

template<typename Tuple, size_t... Indexes>
const Instruction* InstructionRegistry::search_for_ext_opcode_impl_(
                                                        const Tuple& ext_opcodes,
                                                        std::index_sequence<Indexes...>) const {
    const Instruction* result = nullptr;

    // Search for first non nullptr entry and return
    (void) (((result = search_in_registry_<Tuple, Indexes>(ext_opcodes))
                     == nullptr) && ...);

    return result;
}

template<typename... Types>
const Instruction* InstructionRegistry::search_for_ext_opcode_(
                                                const std::tuple<Types...>& ext_opcodes) const {

    return search_for_ext_opcode_impl_(ext_opcodes, std::make_index_sequence<sizeof...(Types)>{});
}

const Instruction* InstructionRegistry::get_instruction(
                                                RawInstruction raw_instruction) const noexcept {

    const ExtendedOpcodeTuple ext_opcodes = ExtendedOpcodesFactory::operator()(raw_instruction);

    return search_for_ext_opcode_(ext_opcodes);
}

