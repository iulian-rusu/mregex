#ifndef MREGEX_AST_TRAITS_HPP
#define MREGEX_AST_TRAITS_HPP

#include <string_view>
#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    /**
     * Type trait to identify nodes that can be trivially matched.
     * A type T is trivially matchable if it always consumes one character when matching.
     * A type T is detected as trivially matchable by checking if it contains a static member function
     * template match_one<A, B>, where A satisfies std::forward_iterator and B is any generic type.
     */
    template<typename Node>
    inline constexpr bool is_trivially_matchable_v = requires { &Node::template match_one<const char *, int>; };

    template<typename... Nodes>
    inline constexpr bool are_trivially_matchable_v = (is_trivially_matchable_v<Nodes> && ...);

    /**
     * Metafunction used to count the number of capturing groups in the regex AST.
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename... Rest>
    inline constexpr std::size_t capture_count_v = First::capture_count + (Rest::capture_count + ... + 0);
}
#endif //MREGEX_AST_TRAITS_HPP