#ifndef MREGEX_AST_TRAITS_HPP
#define MREGEX_AST_TRAITS_HPP

#include <concepts>
#include <mregex/ast/astfwd.hpp>
#include <mregex/utility/type_sequence.hpp>
#include <mregex/utility/type_traits.hpp>

namespace meta::ast
{
    /**
     * Type trait used to detect trivially matchable AST nodes.
     *
     * @note Semantically, a node is considered trivially matchable if it can be matched
     * just by looking at one input character. Additionally, in case of a successful match,
     * the given input character will always be consumed.
     *
     * @note Syntactically, a node is detected as trivially matchable by checking if it
     * contains a static member function with a specific signature.
     * This does not enforce any semantic guarantees on the node and should be treated as
     * a marker trait.
     *
     * @tparam Node The AST node
     */
    template<typename Node>
    inline constexpr bool is_trivially_matchable = requires (char input, type_sequence<> ctx)
    {
        { Node::match_one(input, ctx) } -> std::same_as<bool>;
    };

    template<typename... Nodes>
    inline constexpr bool are_trivially_matchable = (is_trivially_matchable<Nodes> && ...);

    /**
     * Metafunction used to count the number of capturing groups in the regex AST.
     *
     * @tparam Node The AST node
     */
    template<typename Node>
    inline constexpr std::size_t capture_count = 0;

    template<template<typename...> typename Wrapper, typename... Nodes>
    inline constexpr std::size_t capture_count<Wrapper<Nodes ...>> = (capture_count<Nodes> + ... + 0);

    template<match_mode Mode, symbol::finite_quantifier A, symbol::quantifier B, typename Inner>
    inline constexpr std::size_t capture_count<basic_repetition<Mode, A, B, Inner>> = capture_count<Inner>;

    template<lookaround_direction Direction, typename Inner>
    inline constexpr std::size_t capture_count<lookaround<Direction, Inner>> = capture_count<Inner>;

    template<std::size_t ID, typename Name, typename Inner>
    inline constexpr std::size_t capture_count<capture<ID, Name, Inner>> = capture_count<Inner> + 1;

    /**
     * Metafunction that builds a specification for the names of all capturing groups in the AST.
     * The specification lists all capture names according to Preorder AST traversal.
     *
     * @tparam Node The AST node
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

    template<lookaround_direction Direction, typename Inner>
    struct capture_name_spec<lookaround<Direction, Inner>>
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
     *
     * @tparam Node The AST node
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
    inline constexpr bool is_zero_length_matcher<alternation<Nodes ...>> = (is_zero_length_matcher<Nodes> || ...);

    /**
     * Type trait used to identify AST nodes which are assertions.
     *
     * @tparam Node The AST node
     */
    template<typename Node>
    inline constexpr bool is_assertion = is_zero_length_matcher<Node> && requires (char *iter, type_sequence<> ctx)
    {
        typename Node::predicate_type;
        { Node::predicate_type::is_match(iter, iter, iter, ctx) } -> std::same_as<bool>;
    };
}
#endif //MREGEX_AST_TRAITS_HPP