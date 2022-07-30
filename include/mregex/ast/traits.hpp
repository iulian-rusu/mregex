#ifndef MREGEX_AST_TRAITS_HPP
#define MREGEX_AST_TRAITS_HPP

#include <string_view>
#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    /**
     * Type trait to identify nodes that can be trivially matched.
     * A type T is trivially matchable if it always consumes one character when matching.
     * A type T is detected as trivially matchable by checking if it contains a template
     * for a static member function match_one<A, B>, where A satisfies std::forward_iterator
     * and B can be any generic type.
     */
    template<typename Node>
    struct is_trivially_matchable
    {
        using iterator_type = std::string_view::iterator;
        static constexpr bool value = requires { &Node::template match_one<iterator_type, int>; };
    };

    template<typename Node>
    inline constexpr bool is_trivially_matchable_v = is_trivially_matchable<Node>::value;

    template<typename... Nodes>
    inline constexpr bool are_trivially_matchable_v = (is_trivially_matchable_v<Nodes> && ...);

    /**
     * Metafunction used to count the number of capturing groups in the regex AST.
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename... Rest>
    struct capture_count
    {
        static constexpr std::size_t value = First::capture_count + (Rest::capture_count + ... + 0);
    };

    template<typename First, typename... Rest>
    inline constexpr std::size_t capture_count_v = capture_count<First, Rest ...>::value;
}
#endif //MREGEX_AST_TRAITS_HPP