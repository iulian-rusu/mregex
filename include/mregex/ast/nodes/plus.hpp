#ifndef MREGEX_NODES_PLUS_HPP
#define MREGEX_NODES_PLUS_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    template<typename T>
    struct plus : sequence<T, star<T>>
    {
        static constexpr std::size_t capture_count = T::capture_count;
    };
}
#endif //MREGEX_NODES_PLUS_HPP