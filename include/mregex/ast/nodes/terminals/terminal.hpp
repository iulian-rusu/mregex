#ifndef MREGEX_NODES_TERMINAL_HPP
#define MREGEX_NODES_TERMINAL_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    struct terminal
    {
        static constexpr std::size_t capture_count = 0;
    };
}
#endif //MREGEX_NODES_TERMINAL_HPP