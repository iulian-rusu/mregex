#ifndef MREGEX_NODES_PLUS_HPP
#define MREGEX_NODES_PLUS_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    template<match_mode Mode, typename Inner>
    struct basic_plus : sequence<Inner, basic_star<Mode, Inner>>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;
    };
}
#endif //MREGEX_NODES_PLUS_HPP