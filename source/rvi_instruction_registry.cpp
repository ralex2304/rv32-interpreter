#include "rvi_instruction_registry.h"

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"

#include <cassert>
#include <utility>
#include <variant>

using namespace rvi;

void InstructionRegistry::add_instruction(std::unique_ptr<Instruction> instruction) {
    using namespace std::literals;

    ExtendedOpcode ext_opcode = instruction->get_extended_opcode();
    auto instr_name = instruction->get_name();

    std::visit([&registries = registries_, &instruction, &instr_name](const auto& ext_opcode_value) {
        using T = std::decay_t<decltype(ext_opcode_value)>;

        auto& registry = std::get<InstructionRegistryMap<T>>(registries);

        auto [it, inserted] = registry.try_emplace(ext_opcode_value, std::move(instruction));

        if (!inserted)
            throw exception("Error adding instruction \""s + std::string(instr_name) +
                            "\": already exist"s);
    }, ext_opcode);
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

