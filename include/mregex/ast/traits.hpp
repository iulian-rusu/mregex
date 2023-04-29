#ifndef MREGEX_AST_TRAITS_HPP
#define MREGEX_AST_TRAITS_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/utility/type_sequence.hpp>
#include <mregex/utility/type_traits.hpp>

namespace meta::ast
{
    /**
     * Type trait used to identify nodes that can be trivially matched.
     * A node is detected as trivially matchable by checking if it contains a static member function
     * template match_one<A, B>, where A satisfies std::forward_iterator and B is a regular type.
     */
    template<typename Node>
    inline constexpr bool is_trivially_matchable = requires { &Node::template match_one<char *, type_sequence<>>; };

    template<typename... Nodes>
    inline constexpr bool are_trivially_matchable = (is_trivially_matchable<Nodes> && ...);

    /**
     * Metafunction used to count the number of capturing groups in the regex AST.
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename... Rest>
    inline constexpr std::size_t count_captures = First::capture_count + (Rest::capture_count + ... + 0);

    /**
    * Metafunction that builds a specification for the names of all capturing groups in the AST.
    * The specification lists all capture names according to Preorder AST traversal.
    *
    * @tparam Node The AST node with the capturing groups
    */
    template<typename Node>
    struct capture_name_spec
    {
        using type = type_sequence<>;
    };

    template<typename Node>
    using capture_name_spec_t = typename capture_name_spec<Node>::type;

    template<template<typename...> typename Wrapper, typename... Nodes>
    struct capture_name_spec<Wrapper<Nodes ...>>
    {
        using type = concat_t<capture_name_spec_t<Nodes> ...>;
    };

    template<match_mode Mode, symbol::finite_quantifier A, symbol::quantifier B, typename Inner>
    struct capture_name_spec<basic_repetition<Mode, A, B, Inner>>
    {
        using type = capture_name_spec_t<Inner>;
    };

    template<assertion_mode Mode, lookaround_direction Direction, typename Inner>
    struct capture_name_spec<lookaround<Mode, Direction, Inner>>
    {
        using type = capture_name_spec_t<Inner>;
    };

    template<std::size_t ID, typename Name, typename Inner>
    struct capture_name_spec<capture<ID, Name, Inner>>
    {
        using type = push_t<capture_name_spec_t<Inner>, Name>;
    };

    /**
     * Base type used to tag all AST nodes which are zero-length matchers.
     * A node is a zero-length matcher if it may match inputs of length zero.
     */
    struct zero_length_matcher {};

    /**
     * Type trait used to identify zero-length matchers.
     */
    template<typename Node>
    inline constexpr bool is_zero_length_matcher = std::is_base_of_v<zero_length_matcher, Node>;

    template<std::size_t ID, typename Name, typename Inner>
    inline constexpr bool is_zero_length_matcher<capture<ID, Name, Inner>> = is_zero_length_matcher<Inner>;

    template<match_mode Mode, symbol::finite_quantifier A, symbol::quantifier B, typename Inner>
    inline constexpr bool is_zero_length_matcher<basic_repetition<Mode, A, B, Inner>> = symbol::is_zero<A> || is_zero_length_matcher<Inner>;

    template<typename... Nodes>
    inline constexpr bool is_zero_length_matcher<sequence<Nodes ...>> = (is_zero_length_matcher<Nodes> && ...);

    template<typename... Nodes>
    inline constexpr bool is_zero_length_matcher<alternation<Nodes ...>> = (is_zero_length_matcher<Nodes> && ...);
}
#endif //MREGEX_AST_TRAITS_HPP