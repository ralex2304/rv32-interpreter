#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"

#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>

namespace rvi {

class InstructionRegistry {
    public:
        void add_instruction(std::unique_ptr<Instruction> instruction);

        const Instruction* get_instruction(RawInstruction raw_instruction) const noexcept;

        class exception: public std::runtime_error {
            using std::runtime_error::runtime_error;
        };

    private:
        template<typename Tuple>
        struct GetInstructionRegistriesMaps_;

        template<typename... Keys>
        struct GetInstructionRegistriesMaps_<std::tuple<Keys...>> {
             using type = std::tuple<std::unordered_map<Keys, std::unique_ptr<Instruction>,
                                                        ExtendedOpcodeHasher<Keys>>...>;
        };

        using InstructionRegistriesMaps = typename GetInstructionRegistriesMaps_
                                                                    <ExtendedOpcodeTuple>::type;

        InstructionRegistriesMaps registries_;

        template<typename Variant, size_t... Indexes>
        bool try_emplace_instruction_impl_(const Variant& ext_opcode,
                                           std::unique_ptr<Instruction> instruction,
                                           std::index_sequence<Indexes...>);

        template<typename... Types>
        bool try_emplace_instruction_(const std::variant<Types...>& ext_opcode,
                                      std::unique_ptr<Instruction> instruction);

        template<typename Tuple, size_t Index>
        const Instruction* search_in_registry_(const Tuple& ext_opcodes) const;

        template<typename Tuple, size_t... Indexes>
        const Instruction* search_for_ext_opcode_impl_(const Tuple& ext_opcodes,
                                                       std::index_sequence<Indexes...>) const;

        template<typename... Types>
        const Instruction* search_for_ext_opcode_(const std::tuple<Types...>& ext_opcodes) const;
};

} // namespace rvi

