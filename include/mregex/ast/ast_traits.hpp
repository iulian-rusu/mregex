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
    template<typename T>
    struct is_trivially_matchable
    {
        using iterator_type = std::string_view::iterator;

        static constexpr bool value = requires { &T::template match_one<iterator_type, int>; };
    };

    template<typename T>
    inline constexpr bool is_trivially_matchable_v = is_trivially_matchable<T>::value;

    template<typename... Elems>
    inline constexpr bool are_trivially_matchable_v = (is_trivially_matchable_v<Elems> && ...);

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

    /**
     * Metafunction used to get the maximum number of capturing groups in any AST node.
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename... Rest>
    struct max_capture_count
    {
        static constexpr std::size_t first_count = First::capture_count;
        static constexpr std::size_t rest_count = max_capture_count<Rest ...>::value;
        static constexpr std::size_t value = first_count > rest_count ? first_count : rest_count;
    };

    template<typename First>
    struct max_capture_count<First>
    {
        static constexpr std::size_t value = First::capture_count;
    };

    template<typename First, typename... Rest>
    inline constexpr std::size_t max_capture_count_v = max_capture_count<First, Rest ...>::value;

    /**
     * Metafunction used to add a prefix node to an AST.
     *
     * @tparam AST  The Abstract Syntax Tree of the regular expression
     * @tparam Node The prefix node to be added
     */
    template<typename AST, typename Node>
    struct add_prefix
    {
        using type = sequence<Node, AST>;
    };

    template<typename Node, typename... Elems>
    struct add_prefix<sequence<Elems ...>, Node>
    {
        using type = sequence<Node, Elems ...>;
    };

    template<typename AST, typename Node>
    using add_prefix_t = typename add_prefix<AST, Node>::type;

    /**
     * Metafunction that transforms an AST to be suitable for searching.
     * Such an AST has a prefix node which ignores any characters.
     */
    template<typename AST>
    using make_search_ast = add_prefix_t<AST, ignore_prefix>;
}
#endif //MREGEX_AST_TRAITS_HPP